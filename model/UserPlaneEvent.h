/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef UserPlaneEvent_H_
#define UserPlaneEvent_H_

#include <nlohmann/json.hpp>

#include "UserPlaneEvent_anyOf.h"

namespace oai::_3gpp::model {

class UserPlaneEvent {
 public:
  UserPlaneEvent();
  virtual ~UserPlaneEvent() = default;

  void validate() const;
  bool validate(std::stringstream& msg) const;
  bool validate(std::stringstream& msg, const std::string& pathPrefix) const;

  bool operator==(const UserPlaneEvent& rhs) const;
  bool operator!=(const UserPlaneEvent& rhs) const;

  UserPlaneEvent_anyOf getValue() const;
  void setValue(UserPlaneEvent_anyOf value);
  UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf getEnumValue() const;
  void setEnumValue(UserPlaneEvent_anyOf::eUserPlaneEvent_anyOf value);

  friend void to_json(nlohmann::json& j, const UserPlaneEvent& o);
  friend void from_json(const nlohmann::json& j, UserPlaneEvent& o);
  friend void to_json(nlohmann::json& j, const UserPlaneEvent_anyOf& o);
  friend void from_json(const nlohmann::json& j, UserPlaneEvent_anyOf& o);

 protected:
  UserPlaneEvent_anyOf m_value;
};

}  // namespace oai::_3gpp::model

#endif /* UserPlaneEvent_H_ */
