/* Stub for missing auto-generated model file. */
#ifndef ReachabilityType_H_
#define ReachabilityType_H_
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
namespace oai::model {
class ReachabilityType {
 public:
  ReachabilityType()          = default;
  virtual ~ReachabilityType() = default;
  void validate() const {}
  bool validate(std::stringstream&) const { return true; }
  bool validate(std::stringstream&, const std::string&) const { return true; }
  bool operator==(const ReachabilityType& rhs) const {
    return m_value == rhs.m_value;
  }
  bool operator!=(const ReachabilityType& rhs) const { return !(*this == rhs); }
  std::string getValue() const { return m_value; }
  void setValue(const std::string& v) { m_value = v; }
  friend void to_json(nlohmann::json& j, const ReachabilityType& o) {
    j = o.m_value;
  }
  friend void from_json(const nlohmann::json& j, ReachabilityType& o) {
    if (j.is_string()) o.m_value = j.get<std::string>();
  }

 private:
  std::string m_value;
};
}  // namespace oai::model
#endif
