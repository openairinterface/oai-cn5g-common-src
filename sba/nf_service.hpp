/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_NF_SERVICE_SEEN
#define FILE_NF_SERVICE_SEEN

#include <map>
#include <thread>
#include <mutex>

#include "nf_event.hpp"
#include "sbi_helper.hpp"
#include "http_client.hpp"

namespace oai::sba {

class nf_service {
 public:
  nf_service(
      const std::shared_ptr<nf_event>& ev,
      const std::shared_ptr<oai::sba::http_client>& client_inst);
  nf_service(nf_service const&) = delete;
  virtual ~nf_service();
  void operator=(nf_service const&) = delete;

  void generate_uuid();

  /*
   * Trigger NF instance registration to NRF
   * @param [void]
   * @return void
   */
  void register_to_nrf(
      const oai::common::sbi::nf_addr_t& nrf_addr,
      const nlohmann::json& nf_profile);

  /*
   * Trigger NF instance deregistration to NRF
   * @param [void]
   * @return void
   */
  void deregister_to_nrf(const oai::common::sbi::nf_addr_t& nrf_addr);

  /*
   * Start event nf heartbeat procedure
   * @param [void]
   * @return void
   */
  void start_event_nf_heartbeat(std::string& nrf_uri);

  /*
   * Trigger NF heartbeat procedure
   * @param [void]
   * @return void
   */
  void trigger_nf_heartbeat_procedure(
      uint64_t ms, const oai::common::sbi::nf_addr_t& nrf_addr);

  /*
   * Start event nrf registration retry
   * @param [void]
   * @return void
   */
  void start_nrf_registration_retry();

  /*
   * Trigger NF registration procedure
   * @param [void]
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
   * Discover an NF endpoint via NRF, selecting the NFService whose serviceName
   * matches @service_name (e.g. "namf-evts", "nsmf-event-exposure").
   * @param [const std::string&] target_nf_type: e.g. "AMF", "SMF"
   * @param [const std::string&] service_name: target service name
   * @param [std::string&] endpoint: resolved "scheme://ipv4:port" on success
   * @return true on success, false otherwise
   */
  bool discover_nf(
      const std::string& target_nf_type, const std::string& service_name,
      std::string& endpoint);

 private:
  std::shared_ptr<oai::sba::nf_event> event_sub_;
  std::shared_ptr<oai::sba::http_client> http_client_inst_;

  bs2::connection task_connection;
  bs2::connection retry_nrf_registration_task_connection;
  std::string nf_instance_id;  // NF instance id
  // Discovery cache keyed by "<nf_type>:<service_name>"
  std::map<std::string, std::string> m_discovery_cache;
  std::mutex m_discovery_mutex;
};
}  // namespace oai::sba
#endif /* FILE_NF_SERVICE_SEEN */
