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
 * PaAfEvent.h
 *
 * Represents an event to notify to the AF (TS 29.514 §5.6.3.4).
 */

#ifndef PaAfEvent_H_
#define PaAfEvent_H_

#include "PaAfEvent_anyOf.h"
#include <nlohmann/json.hpp>

namespace oai::_3gpp::model {

/// <summary>
/// Represents an event to notify to the AF.
/// </summary>
class PaAfEvent {
 public:
  PaAfEvent();
  virtual ~PaAfEvent() = default;

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

  bool operator==(const PaAfEvent& rhs) const;
  bool operator!=(const PaAfEvent& rhs) const;

  /////////////////////////////////////////////
  /// PaAfEvent members

  PaAfEvent_anyOf getValue() const;
  void setValue(PaAfEvent_anyOf value);
  PaAfEvent_anyOf::ePaAfEvent_anyOf getEnumValue() const;
  void setEnumValue(PaAfEvent_anyOf::ePaAfEvent_anyOf value);
  friend void to_json(nlohmann::json& j, const PaAfEvent& o);
  friend void from_json(const nlohmann::json& j, PaAfEvent& o);
  friend void to_json(nlohmann::json& j, const PaAfEvent_anyOf& o);
  friend void from_json(const nlohmann::json& j, PaAfEvent_anyOf& o);

 protected:
  PaAfEvent_anyOf m_value;
};

}  // namespace oai::_3gpp::model

#endif /* PaAfEvent_H_ */
