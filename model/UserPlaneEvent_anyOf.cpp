/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "UserPlaneEvent_anyOf.h"

#include <sstream>
#include <stdexcept>

#include "Helpers.h"

namespace oai::_3gpp::model {

UserPlaneEvent_anyOf::UserPlaneEvent_anyOf() {}

void UserPlaneEvent_anyOf::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw oai::_3gpp::model::helpers::ValidationException(msg.str());
  }
}

bool UserPlaneEvent_anyOf::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool UserPlaneEvent_anyOf::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "UserPlaneEvent_anyOf" : pathPrefix;

  if (m_value == UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::
                     INVALID_VALUE_OPENAPI_GENERATED) {
    success = false;
    msg << _pathPrefix << ": has no value;";
  }

  return success;
}

bool UserPlaneEvent_anyOf::operator==(const UserPlaneEvent_anyOf& rhs) const {
  return getValue() == rhs.getValue();
}

bool UserPlaneEvent_anyOf::operator!=(const UserPlaneEvent_anyOf& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const UserPlaneEvent_anyOf& o) {
  j = nlohmann::json();

  switch (o.getValue()) {
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::
        INVALID_VALUE_OPENAPI_GENERATED:
      j = "INVALID_VALUE_OPENAPI_GENERATED";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::SESSION_TERMINATION:
      j = "SESSION_TERMINATION";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::LOSS_OF_BEARER:
      j = "LOSS_OF_BEARER";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::RECOVERY_OF_BEARER:
      j = "RECOVERY_OF_BEARER";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::RELEASE_OF_BEARER:
      j = "RELEASE_OF_BEARER";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::USAGE_REPORT:
      j = "USAGE_REPORT";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::
        FAILED_RESOURCES_ALLOCATION:
      j = "FAILED_RESOURCES_ALLOCATION";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::QOS_GUARANTEED:
      j = "QOS_GUARANTEED";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::QOS_NOT_GUARANTEED:
      j = "QOS_NOT_GUARANTEED";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::QOS_MONITORING:
      j = "QOS_MONITORING";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::
        SUCCESSFUL_RESOURCES_ALLOCATION:
      j = "SUCCESSFUL_RESOURCES_ALLOCATION";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::ACCESS_TYPE_CHANGE:
      j = "ACCESS_TYPE_CHANGE";
      break;
    case UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::PLMN_CHG:
      j = "PLMN_CHG";
      break;
  }
}

void from_json(const nlohmann::json& j, UserPlaneEvent_anyOf& o) {
  auto s = j.get<std::string>();
  if (s == "SESSION_TERMINATION") {
    o.setValue(
        UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::SESSION_TERMINATION);
  } else if (s == "LOSS_OF_BEARER") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::LOSS_OF_BEARER);
  } else if (s == "RECOVERY_OF_BEARER") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::RECOVERY_OF_BEARER);
  } else if (s == "RELEASE_OF_BEARER") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::RELEASE_OF_BEARER);
  } else if (s == "USAGE_REPORT") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::USAGE_REPORT);
  } else if (s == "FAILED_RESOURCES_ALLOCATION") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::
                   FAILED_RESOURCES_ALLOCATION);
  } else if (s == "QOS_GUARANTEED") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::QOS_GUARANTEED);
  } else if (s == "QOS_NOT_GUARANTEED") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::QOS_NOT_GUARANTEED);
  } else if (s == "QOS_MONITORING") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::QOS_MONITORING);
  } else if (s == "SUCCESSFUL_RESOURCES_ALLOCATION") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::
                   SUCCESSFUL_RESOURCES_ALLOCATION);
  } else if (s == "ACCESS_TYPE_CHANGE") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::ACCESS_TYPE_CHANGE);
  } else if (s == "PLMN_CHG") {
    o.setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf::PLMN_CHG);
  } else {
    std::stringstream ss;
    ss << "Unexpected value " << s << " in json"
       << " cannot be converted to enum of type"
       << " UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf";
    throw std::invalid_argument(ss.str());
  }
}

UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf UserPlaneEvent_anyOf::getValue()
    const {
  return m_value;
}

void UserPlaneEvent_anyOf::setValue(
    UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf value) {
  m_value = value;
}

}  // namespace oai::_3gpp::model
