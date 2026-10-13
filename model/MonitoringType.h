/*
 * MonitoringType.h
 * Stub for missing auto-generated model file.
 */
#ifndef MonitoringType_H_
#define MonitoringType_H_

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

namespace oai::model {

class MonitoringType {
 public:
  MonitoringType()          = default;
  virtual ~MonitoringType() = default;

  void validate() const {}
  bool validate(std::stringstream& /*msg*/) const { return true; }
  bool validate(
      std::stringstream& /*msg*/, const std::string& /*pathPrefix*/) const {
    return true;
  }

  bool operator==(const MonitoringType& rhs) const {
    return m_value == rhs.m_value;
  }
  bool operator!=(const MonitoringType& rhs) const { return !(*this == rhs); }

  std::string getValue() const { return m_value; }
  void setValue(const std::string& v) { m_value = v; }

  friend void to_json(nlohmann::json& j, const MonitoringType& o) {
    j = o.m_value;
  }
  friend void from_json(const nlohmann::json& j, MonitoringType& o) {
    if (j.is_string()) o.m_value = j.get<std::string>();
  }

 private:
  std::string m_value;
};

}  // namespace oai::model

#endif /* MonitoringType_H_ */
