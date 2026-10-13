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
/*
 * PaAfEvent_anyOf.h
 *
 *
 */

#ifndef PaAfEvent_anyOf_H_
#define PaAfEvent_anyOf_H_

#include <nlohmann/json.hpp>

namespace oai::_3gpp::model {

/// <summary>
///
/// </summary>
class PaAfEvent_anyOf {
 public:
  PaAfEvent_anyOf();
  virtual ~PaAfEvent_anyOf() = default;

  enum class ePaAfEvent_anyOf {
    // To have a valid default value.
    // Avoiding name clashes with user defined
    // enum values
    INVALID_VALUE_OPENAPI_GENERATED = 0,
    ACCESS_TYPE_CHANGE,
    ANI_REPORT,
    APP_DETECTION,
    CHARGING_CORRELATION,
    EPS_FALLBACK,
    FAILED_QOS_UPDATE,
    FAILED_RESOURCES_ALLOCATION,
    OUT_OF_CREDIT,
    PDU_SESSION_STATUS,
    PLMN_CHG,
    QOS_MONITORING,
    QOS_NOTIF,
    RAN_NAS_CAUSE,
    REALLOCATION_OF_CREDIT,
    SAT_CATEGORY_CHG,
    SUCCESSFUL_QOS_UPDATE,
    SUCCESSFUL_RESOURCES_ALLOCATION,
    TSN_BRIDGE_INFO,
    UP_PATH_CHG_FAILURE,
    USAGE_REPORT
  };

  /// <summary>
  /// Validate the current data in the model. Throws a ValidationException on
  /// failure.
  /// </summary>
  void validate() const;

  /// <summary>
  /// Validate the current data in the model. Returns false on error and writes
  /// an error message into the given stringstream.
  /// </summary>
  bool validate(std::stringstream& msg) const;

  /// <summary>
  /// Helper overload for validate. Used when one model stores another model and
  /// calls it's validate. Not meant to be called outside that case.
  /// </summary>
  bool validate(std::stringstream& msg, const std::string& pathPrefix) const;

  bool operator==(const PaAfEvent_anyOf& rhs) const;
  bool operator!=(const PaAfEvent_anyOf& rhs) const;

  /////////////////////////////////////////////
  /// PaAfEvent_anyOf members

  PaAfEvent_anyOf::ePaAfEvent_anyOf getValue() const;
  void setValue(PaAfEvent_anyOf::ePaAfEvent_anyOf value);

  friend void to_json(nlohmann::json& j, const PaAfEvent_anyOf& o);
  friend void from_json(const nlohmann::json& j, PaAfEvent_anyOf& o);

 protected:
  PaAfEvent_anyOf::ePaAfEvent_anyOf m_value =
      PaAfEvent_anyOf::ePaAfEvent_anyOf::INVALID_VALUE_OPENAPI_GENERATED;
};

}  // namespace oai::_3gpp::model

#endif /* PaAfEvent_anyOf_H_ */
