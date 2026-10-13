/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_NF_EVENT_HPP_SEEN
#define FILE_NF_EVENT_HPP_SEEN

#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

#include "task_manager.hpp"

namespace oai::sba {

typedef bs2::signal_type<
    void(uint64_t), bs2::keywords::mutex_type<bs2::dummy_mutex>>::type
    task_sig_t;

class task_manager;
class nf_event {
 public:
  nf_event() {};
  nf_event(nf_event const&)       = delete;
  void operator=(nf_event const&) = delete;
  virtual ~nf_event()             = default;

  static nf_event& get_instance() {
    static nf_event instance;
    return instance;
  }

  friend class task_manager;

  /*
   * Subscribe to the task tick event
   * @param [const task_sig_t::slot_type &] sig
   * @param [uint64_t] period: interval between two events
   * @param [uint64_t] start:
   * @return void
   */
  virtual bs2::connection subscribe_task_nf_heartbeat(
      const task_sig_t::slot_type& sig, uint64_t period, uint64_t start = 0);

  virtual void notify_task_tick(uint64_t tick);

 private:
  task_sig_t task_tick;
};
}  // namespace oai::sba
#endif
