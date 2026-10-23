/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef UserPlaneEvent_anyOf_H_
#define UserPlaneEvent_anyOf_H_

#include <nlohmann/json.hpp>

namespace oai::_3gpp::model {

class UserPlaneEvent_anyOf {
 public:
  UserPlaneEvent_anyOf();
  virtual ~UserPlaneEvent_anyOf() = default;

  enum class eUserPlaneEvent_anyOf {
    INVALID_VALUE_OPENAPI_GENERATED = 0,
    SESSION_TERMINATION,
    LOSS_OF_BEARER,
    RECOVERY_OF_BEARER,
    RELEASE_OF_BEARER,
    USAGE_REPORT,
    FAILED_RESOURCES_ALLOCATION,
    QOS_GUARANTEED,
    QOS_NOT_GUARANTEED,
    QOS_MONITORING,
    SUCCESSFUL_RESOURCES_ALLOCATION,
    ACCESS_TYPE_CHANGE,
    PLMN_CHG
  };

  void validate() const;
  bool validate(std::stringstream& msg) const;
  bool validate(std::stringstream& msg, const std::string& pathPrefix) const;

  bool operator==(const UserPlaneEvent_anyOf& rhs) const;
  bool operator!=(const UserPlaneEvent_anyOf& rhs) const;

  UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf getValue() const;
  void setValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf value);

  friend void to_json(nlohmann::json& j, const UserPlaneEvent_anyOf& o);
  friend void from_json(const nlohmann::json& j, UserPlaneEvent_anyOf& o);

 protected:
  UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf m_value = UserPlaneEvent_anyOf::
      eUserPlaneEvent_anyOf::INVALID_VALUE_OPENAPI_GENERATED;
};

}  // namespace oai::_3gpp::model

#endif /* UserPlaneEvent_anyOf_H_ */
