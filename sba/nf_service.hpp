/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_NF_SERVICE_SEEN
#define FILE_NF_SERVICE_SEEN

#include <chrono>
#include <map>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include "nf_event.hpp"
#include "sbi_helper.hpp"
#include "http_client.hpp"

namespace oai::sba {

// Delay, in seconds, between two NRF registration attempts once the first one
// has failed. Matches the value the NFs that predate this class use.
constexpr uint64_t kNrfRegistrationRetryTimerSeconds = 5;

// Interval, in seconds, between two NF heartbeats towards the NRF.
constexpr uint64_t kNfHeartbeatTimerSeconds = 10;

// How long a discovered endpoint stays usable, in seconds. An NF instance may
// move or be scaled away without anyone telling us, so a cached endpoint is
// re-discovered once it reaches this age.
constexpr int kDiscoveryCacheTtlSeconds = 30;

// Which NRF procedure a request belongs to. An NF that overrides
// send_with_policy() gets this so it can apply a different transport policy
// (retry budget, circuit breaker, ...) to each procedure rather than to all
// of them at once.
enum class nrf_call_kind { registration, deregistration, heartbeat, discovery };

class nf_service {
 public:
  nf_service(
      const std::shared_ptr<nf_event>& ev,
      const std::shared_ptr<oai::sba::http_client>& client_inst);
  nf_service(nf_service const&) = delete;
  virtual ~nf_service();
  void operator=(nf_service const&) = delete;

  /*
   * (Re)generate the NF instance UUID of this instance
   * @param [void]
   * @return void
   */
  void generate_uuid();

  /*
   * Trigger NF instance registration to NRF. The NRF address and the NF
   * profile are retained: the heartbeat-failure path and the retry timer both
   * need to re-send this very request, and neither of them is given any
   * argument by the task tick that drives them.
   * @param [const oai::common::sbi::nf_addr_t&] nrf_addr: NRF's address
   * @param [const nlohmann::json&] nf_profile: this NF's NFProfile
   * @return true if the NF is registered (or registration is disabled by
   *         nrf_registration_enabled()), false otherwise
   */
  bool register_to_nrf(
      const oai::common::sbi::nf_addr_t& nrf_addr,
      const nlohmann::json& nf_profile);

  /*
   * Trigger NF instance deregistration to NRF
   * @return true on a 204 No Content from the NRF (or if registration is
   *         disabled by nrf_registration_enabled()), false otherwise
   */
  bool deregister_to_nrf();

  /*
   * Start event nf heartbeat procedure.
   * The interval is a parameter rather than a fixed constant because an NF
   * advertises its own heartBeatTimer in the NFProfile it registers, and the
   * two must agree: ticking faster than advertised multiplies NRF traffic,
   * ticking slower risks the NRF marking the instance SUSPENDED.
   * @param [uint64_t] heartbeat_seconds: interval, default
   *        kNfHeartbeatTimerSeconds
   * @return void
   */
  void start_event_nf_heartbeat(
      uint64_t heartbeat_seconds = kNfHeartbeatTimerSeconds);

  /*
   * Trigger NF heartbeat procedure
   * @param [uint64_t] ms: current time, supplied by the task tick
   * @return void
   */
  void trigger_nf_heartbeat_procedure(uint64_t ms);

  /*
   * Start event nrf registration retry
   * @param [void]
   * @return void
   */
  void start_nrf_registration_retry();

  /*
   * Trigger NF registration procedure
   * @param [uint64_t] ms: current time, supplied by the task tick
   * @return void
   */
  void trigger_nrf_registration_retry_procedure(uint64_t ms);

  /*
   * Stop event nrf registration retry
   * @param [void]
   * @return void
   */
  void stop_nrf_registration_retry();

  /*
   * Generate a NF profile for this instance
   * @param [void]
   * @return void
   */
  void generate_nf_profile();

  /*
   * Discover an NF endpoint, selecting the NFService whose serviceName matches
   * @service_name (e.g. "namf-evts", "nsmf-event-exposure"); an empty
   * @service_name means "any service", i.e. the first one the instance lists.
   * The NRF address and the requester NF type are explicit rather than taken
   * from the registration state, so that discovery also works for an NF that
   * never registers.
   *
   * The procedure is: local configuration (resolve_endpoint_from_config) ->
   * the enablement gate (nrf_discovery_enabled) -> the cache
   * (discovery_cache_lookup) -> the NRF SearchNFInstances query. Every step is
   * a protected hook, so a derived NF supplies the policy and this class keeps
   * the procedure.
   * @param [const oai::common::sbi::nf_addr_t&] nrf_addr: NRF's address
   * @param [const std::string&] requester_nf_type: e.g. "NEF", "UDM"
   * @param [const std::string&] target_nf_type: e.g. "AMF", "SMF"
   * @param [const std::string&] service_name: target service name
   * @param [std::string&] endpoint: resolved "scheme://ipv4:port" on success
   * @return true on success, false otherwise
   */
  bool discover_nf(
      const oai::common::sbi::nf_addr_t& nrf_addr,
      const std::string& requester_nf_type, const std::string& target_nf_type,
      const std::string& service_name, std::string& endpoint);

 protected:
  /*
   * Send the NF registration request built from the retained NRF address and
   * NF profile. Used by register_to_nrf() and by both re-registration paths
   * (heartbeat failure, retry timer).
   * @param [void]
   * @return true if the registration succeeded (or is disabled), false
   *         otherwise
   */
  bool send_nf_registration();

  // ---------------------------------------------------------------------
  // Policy hooks. This class owns the NRF procedures; a derived NF overrides
  // the hooks below to supply its own policy. Every default reproduces the
  // behaviour this class had before the hooks existed, so an NF that
  // overrides nothing is unaffected.
  // ---------------------------------------------------------------------

  /*
   * Is this NF supposed to talk to the NRF at all? Checked by
   * send_nf_registration() (hence by the retry and heartbeat-failure paths
   * too) and by deregister_to_nrf(). An NF whose configuration can switch NRF
   * registration off overrides this; when it returns false both calls no-op
   * and report success.
   */
  virtual bool nrf_registration_enabled() const { return true; }

  /*
   * Same question for discovery, which some deployments disable while still
   * registering (or resolve statically instead).
   */
  virtual bool nrf_discovery_enabled() const { return true; }

  /*
   * Resolve @target_nf_type / @service_name from local configuration, before
   * the NRF is consulted. Deployments that pin peer addresses statically need
   * this ordering. Default: nothing is configured statically.
   * @return true if @endpoint was filled in
   */
  virtual bool resolve_endpoint_from_config(
      const std::string& target_nf_type, const std::string& service_name,
      std::string& endpoint) {
    (void) target_nf_type;
    (void) service_name;
    (void) endpoint;
    return false;
  }

  /*
   * Look @target_nf_type / @service_name up in the discovery cache. Default:
   * the m_discovery_cache below, where an entry older than
   * discovery_cache_ttl_seconds() counts as a miss. An NF that resolves
   * endpoints some other way overrides this.
   * @return true if @endpoint was filled in
   */
  virtual bool discovery_cache_lookup(
      const std::string& target_nf_type, const std::string& service_name,
      std::string& endpoint);

  /*
   * Store an endpoint in the discovery cache, replacing any existing one.
   * Called by the default handle_discovery_response(), and by a derived NF
   * that selects the instance itself but still wants the base cache.
   * @param [int] ttl_seconds: lifetime, default discovery_cache_ttl_seconds().
   *        0 expires the entry immediately, which is what tests want.
   */
  void discovery_cache_store(
      const std::string& target_nf_type, const std::string& service_name,
      std::string endpoint, int ttl_seconds);
  void discovery_cache_store(
      const std::string& target_nf_type, const std::string& service_name,
      std::string endpoint) {
    discovery_cache_store(
        target_nf_type, service_name, std::move(endpoint),
        discovery_cache_ttl_seconds());
  }

  /*
   * Drop a cached endpoint so the next discover_nf() re-queries the NRF.
   * Worth calling when a cached endpoint answers with a connection error or a
   * 503: the NF has probably moved.
   */
  void discovery_cache_invalidate(
      const std::string& target_nf_type, const std::string& service_name = {});

  /*
   * Empty the discovery cache. Mainly for test teardown.
   */
  void discovery_cache_clear();

  /*
   * Lifetime of a cache entry. An NF whose peers move more (or less) often
   * than every kDiscoveryCacheTtlSeconds overrides this.
   */
  virtual int discovery_cache_ttl_seconds() const {
    return kDiscoveryCacheTtlSeconds;
  }

  /*
   * Turn a 200 OK SearchResult into an endpoint, and cache it. Both halves
   * are one hook because an NF that keeps its own cache also tends to decide
   * for itself which instance and which service to select.
   * @return true if @endpoint was filled in
   */
  virtual bool handle_discovery_response(
      const oai::sba::response& search_result_resp,
      const std::string& target_nf_type, const std::string& service_name,
      std::string& endpoint);

  /*
   * Send one NRF request. Default: a single unconditional send. An NF with a
   * retry budget or a circuit breaker overrides this and may vary the policy
   * by @kind -- registration and discovery are usually worth retrying,
   * deregistration on shutdown usually is not.
   */
  virtual oai::sba::response send_with_policy(
      nrf_call_kind kind, const oai::common::sbi::method_e& method,
      const oai::sba::request& req);

  /*
   * The port to assume when a discovered ipEndPoint carries none, and the one
   * appended to a bare instance-level ipv4Address. Default: 80.
   */
  virtual uint16_t default_sbi_port() const { return 80; }

  /*
   * Did the NRF accept the registration? Default: a 200/201 whose body says
   * nfStatus == "REGISTERED". An NF that registers against an NRF which
   * answers 201 with an empty body overrides this.
   */
  virtual bool registration_succeeded(const oai::sba::response& resp) const;

  /*
   * Called once per registration attempt with its outcome. Default: arm the
   * heartbeat and cancel the retry timer on success, arm the retry timer on
   * failure. An NF whose application layer owns those timers overrides this
   * with its own logging and nothing else.
   */
  virtual void on_registration_outcome(
      bool success, const oai::sba::response& resp);

  std::shared_ptr<oai::sba::nf_event> event_sub_;
  std::shared_ptr<oai::sba::http_client> http_client_inst_;

  bs2::connection task_connection;
  bs2::connection retry_nrf_registration_task_connection;
  std::string nf_instance_id;  // NF instance id
  // NRF address and NF profile of the last register_to_nrf() call, replayed by
  // the heartbeat-failure and retry paths.
  std::optional<oai::common::sbi::nf_addr_t> nrf_addr_ = std::nullopt;
  std::optional<nlohmann::json> nf_profile_            = std::nullopt;
  // Discovery cache keyed by "<nf_type>:<service_name>". Shared between the
  // blocking discover_nf() and whatever asynchronous discovery path a derived
  // NF adds, so reads take the lock in shared mode.
  struct discovery_cache_entry {
    std::string endpoint;
    std::chrono::steady_clock::time_point expires_at;
  };
  std::map<std::string, discovery_cache_entry> m_discovery_cache;
  mutable std::shared_mutex m_discovery_mutex;
};
}  // namespace oai::sba
#endif /* FILE_NF_SERVICE_SEEN */
