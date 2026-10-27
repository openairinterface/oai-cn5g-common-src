/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "UserPlaneEvent.h"

#include <sstream>

#include "Helpers.h"

namespace oai::_3gpp::model {

UserPlaneEvent::UserPlaneEvent() {}

void UserPlaneEvent::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw oai::_3gpp::model::helpers::ValidationException(msg.str());
  }
}

bool UserPlaneEvent::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool UserPlaneEvent::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "UserPlaneEvent" : pathPrefix;

  if (!m_value.validate(msg, _pathPrefix + ".value")) {
    success = false;
  }
  return success;
}

bool UserPlaneEvent::operator==(const UserPlaneEvent& rhs) const {
  return getValue() == rhs.getValue();
}

bool UserPlaneEvent::operator!=(const UserPlaneEvent& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const UserPlaneEvent& o) {
  j = nlohmann::json();
  to_json(j, o.m_value);
}

void from_json(const nlohmann::json& j, UserPlaneEvent& o) {
  from_json(j, o.m_value);
}

UserPlaneEvent_anyOf UserPlaneEvent::getValue() const {
  return m_value;
}

void UserPlaneEvent::setValue(UserPlaneEvent_anyOf value) {
  m_value = value;
}

UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf UserPlaneEvent::getEnumValue()
    const {
  return m_value.getValue();
}

void UserPlaneEvent::setEnumValue(
    UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf value) {
  m_value.setValue(value);
}

}  // namespace oai::_3gpp::model
