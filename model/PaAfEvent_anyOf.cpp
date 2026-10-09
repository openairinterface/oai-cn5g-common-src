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

#include "PaAfEvent_anyOf.h"
#include "Helpers.h"
#include <stdexcept>
#include <sstream>

namespace oai::_3gpp::model {

PaAfEvent_anyOf::PaAfEvent_anyOf() {}

void PaAfEvent_anyOf::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw oai::_3gpp::model::helpers::ValidationException(msg.str());
  }
}

bool PaAfEvent_anyOf::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool PaAfEvent_anyOf::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "PaAfEvent_anyOf" : pathPrefix;

  if (m_value ==
      PaAfEvent_anyOf::ePaAfEvent_anyOf::INVALID_VALUE_OPENAPI_GENERATED) {
    success = false;
    msg << _pathPrefix << ": has no value;";
  }

  return success;
}

bool PaAfEvent_anyOf::operator==(const PaAfEvent_anyOf& rhs) const {
  return getValue() == rhs.getValue()

      ;
}

bool PaAfEvent_anyOf::operator!=(const PaAfEvent_anyOf& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const PaAfEvent_anyOf& o) {
  j = nlohmann::json();

  switch (o.getValue()) {
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::INVALID_VALUE_OPENAPI_GENERATED:
      j = "INVALID_VALUE_OPENAPI_GENERATED";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::ACCESS_TYPE_CHANGE:
      j = "ACCESS_TYPE_CHANGE";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::ANI_REPORT:
      j = "ANI_REPORT";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::APP_DETECTION:
      j = "APP_DETECTION";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::CHARGING_CORRELATION:
      j = "CHARGING_CORRELATION";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::EPS_FALLBACK:
      j = "EPS_FALLBACK";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::FAILED_QOS_UPDATE:
      j = "FAILED_QOS_UPDATE";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::FAILED_RESOURCES_ALLOCATION:
      j = "FAILED_RESOURCES_ALLOCATION";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::OUT_OF_CREDIT:
      j = "OUT_OF_CREDIT";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::PDU_SESSION_STATUS:
      j = "PDU_SESSION_STATUS";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::PLMN_CHG:
      j = "PLMN_CHG";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::QOS_MONITORING:
      j = "QOS_MONITORING";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::QOS_NOTIF:
      j = "QOS_NOTIF";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::RAN_NAS_CAUSE:
      j = "RAN_NAS_CAUSE";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::REALLOCATION_OF_CREDIT:
      j = "REALLOCATION_OF_CREDIT";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::SAT_CATEGORY_CHG:
      j = "SAT_CATEGORY_CHG";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::SUCCESSFUL_QOS_UPDATE:
      j = "SUCCESSFUL_QOS_UPDATE";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::SUCCESSFUL_RESOURCES_ALLOCATION:
      j = "SUCCESSFUL_RESOURCES_ALLOCATION";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::TSN_BRIDGE_INFO:
      j = "TSN_BRIDGE_INFO";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::UP_PATH_CHG_FAILURE:
      j = "UP_PATH_CHG_FAILURE";
      break;
    case PaAfEvent_anyOf::ePaAfEvent_anyOf::USAGE_REPORT:
      j = "USAGE_REPORT";
      break;
  }
}

void from_json(const nlohmann::json& j, PaAfEvent_anyOf& o) {
  auto s = j.get<std::string>();
  if (s == "ACCESS_TYPE_CHANGE") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::ACCESS_TYPE_CHANGE);
  } else if (s == "ANI_REPORT") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::ANI_REPORT);
  } else if (s == "APP_DETECTION") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::APP_DETECTION);
  } else if (s == "CHARGING_CORRELATION") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::CHARGING_CORRELATION);
  } else if (s == "EPS_FALLBACK") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::EPS_FALLBACK);
  } else if (s == "FAILED_QOS_UPDATE") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::FAILED_QOS_UPDATE);
  } else if (s == "FAILED_RESOURCES_ALLOCATION") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::FAILED_RESOURCES_ALLOCATION);
  } else if (s == "OUT_OF_CREDIT") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::OUT_OF_CREDIT);
  } else if (s == "PDU_SESSION_STATUS") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::PDU_SESSION_STATUS);
  } else if (s == "PLMN_CHG") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::PLMN_CHG);
  } else if (s == "QOS_MONITORING") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::QOS_MONITORING);
  } else if (s == "QOS_NOTIF") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::QOS_NOTIF);
  } else if (s == "RAN_NAS_CAUSE") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::RAN_NAS_CAUSE);
  } else if (s == "REALLOCATION_OF_CREDIT") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::REALLOCATION_OF_CREDIT);
  } else if (s == "SAT_CATEGORY_CHG") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::SAT_CATEGORY_CHG);
  } else if (s == "SUCCESSFUL_QOS_UPDATE") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::SUCCESSFUL_QOS_UPDATE);
  } else if (s == "SUCCESSFUL_RESOURCES_ALLOCATION") {
    o.setValue(
        PaAfEvent_anyOf::ePaAfEvent_anyOf::SUCCESSFUL_RESOURCES_ALLOCATION);
  } else if (s == "TSN_BRIDGE_INFO") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::TSN_BRIDGE_INFO);
  } else if (s == "UP_PATH_CHG_FAILURE") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::UP_PATH_CHG_FAILURE);
  } else if (s == "USAGE_REPORT") {
    o.setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf::USAGE_REPORT);
  } else {
    std::stringstream ss;
    ss << "Unexpected value " << s << " in json"
       << " cannot be converted to enum of type"
       << " PaAfEvent_anyOf::ePaAfEvent_anyOf";
    throw std::invalid_argument(ss.str());
  }
}

PaAfEvent_anyOf::ePaAfEvent_anyOf PaAfEvent_anyOf::getValue() const {
  return m_value;
}
void PaAfEvent_anyOf::setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf value) {
  m_value = value;
}

}  // namespace oai::_3gpp::model
