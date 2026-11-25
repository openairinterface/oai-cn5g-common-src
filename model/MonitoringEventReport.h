/* Stub for missing auto-generated model file. */
#ifndef MonitoringEventReport_H_
#define MonitoringEventReport_H_
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
namespace oai::model {
class MonitoringEventReport {
 public:
  MonitoringEventReport()          = default;
  virtual ~MonitoringEventReport() = default;
  void validate() const {}
  bool validate(std::stringstream&) const { return true; }
  bool validate(std::stringstream&, const std::string&) const { return true; }
  bool operator==(const MonitoringEventReport& rhs) const {
    return m_data == rhs.m_data;
  }
  bool operator!=(const MonitoringEventReport& rhs) const {
    return !(*this == rhs);
  }
  friend void to_json(nlohmann::json& j, const MonitoringEventReport& o) {
    j = o.m_data;
  }
  friend void from_json(const nlohmann::json& j, MonitoringEventReport& o) {
    o.m_data = j;
  }

 private:
  nlohmann::json m_data = nlohmann::json::object();
};
}  // namespace oai::model
#endif
