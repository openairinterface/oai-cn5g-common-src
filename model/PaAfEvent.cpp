/**
 * Npcf_PolicyAuthorization Service API
 * PCF Policy Authorization Service.   © 2023, 3GPP Organizational Partners
 * (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC).   All rights reserved.
 *
 * The version of the OpenAPI document: 1.2.3
 *
 *
 * NOTE: This class follows the OpenAPI Generator output shape by hand.
 * TS 29.514 and TS 29.517 both define a schema named `AfEvent`; only the
 * TS 29.517 (Naf_EventExposure) one is generated into this shared model
 * directory as `AfEvent`. This type carries the TS 29.514 §5.6.3.4 values
 * that Npcf_PolicyAuthorization needs, under a distinct name.
 */

#include "PaAfEvent.h"
#include "Helpers.h"

#include <sstream>

namespace oai::_3gpp::model {

PaAfEvent::PaAfEvent() {}

void PaAfEvent::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw oai::_3gpp::model::helpers::ValidationException(msg.str());
  }
}

bool PaAfEvent::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool PaAfEvent::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success                  = true;
  const std::string _pathPrefix = pathPrefix.empty() ? "PaAfEvent" : pathPrefix;

  if (!m_value.validate(msg)) {
    success = false;
  }
  return success;
}

bool PaAfEvent::operator==(const PaAfEvent& rhs) const {
  return

      getValue() == rhs.getValue();
}

bool PaAfEvent::operator!=(const PaAfEvent& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const PaAfEvent& o) {
  j = nlohmann::json();
  to_json(j, o.m_value);
}

void from_json(const nlohmann::json& j, PaAfEvent& o) {
  from_json(j, o.m_value);
}

PaAfEvent_anyOf PaAfEvent::getValue() const {
  return m_value;
}

void PaAfEvent::setValue(PaAfEvent_anyOf value) {
  m_value = value;
}

PaAfEvent_anyOf::ePaAfEvent_anyOf PaAfEvent::getEnumValue() const {
  return m_value.getValue();
}

void PaAfEvent::setEnumValue(PaAfEvent_anyOf::ePaAfEvent_anyOf value) {
  m_value.setValue(value);
}

}  // namespace oai::_3gpp::model
