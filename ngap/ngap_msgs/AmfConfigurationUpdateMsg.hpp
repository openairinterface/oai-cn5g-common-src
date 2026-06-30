/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _AMF_CONFIGURATION_UPDATE_MSG_H_
#define _AMF_CONFIGURATION_UPDATE_MSG_H_

#include <optional>

#include "AmfName.hpp"
#include "ExtendedAmfName.hpp"
#include "NgapMessage.hpp"
#include "PlmnSupportList.hpp"
#include "RelativeAmfCapacity.hpp"
#include "ServedGuamiList.hpp"

extern "C" {
#include "Ngap_AMFConfigurationUpdate.h"
}

namespace oai::ngap {

/**
 * Wrapper for AMFConfigurationUpdate (TS 38.413 §9.2.6.7).
 *
 * Direction: AMF → gNB (encode-only for the Request message).
 * SECURITY: ExtendedAmfName must be sourced from AMF configuration only.
 * TNL association lists are stored as raw generated types; high-level wrapper
 * classes will be added in a later stage.
 */
class AmfConfigurationUpdateMsg : public NgapMessage {
 public:
  AmfConfigurationUpdateMsg();
  virtual ~AmfConfigurationUpdateMsg();

  void initialize();

  // id-AMFName (Optional)
  bool setAmfName(const std::string& name);

  // id-ServedGUAMIList (Optional)
  void setGuamiList(std::vector<struct GuamiItem_s>& list);

  // id-RelativeAMFCapacity (Optional)
  void setRelativeAmfCapacity(uint8_t capacity);

  // id-PLMNSupportList (Optional)
  void setPlmnSupportList(const PlmnSupportList& list);

  // id-ExtendedAMFName (Optional, Rel-17)
  // SECURITY: source from AMF configuration only — never echo from gNB.
  void setExtendedAmfName(const ExtendedAmfName& value);

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_AMFConfigurationUpdate_t* m_AmfConfigurationUpdateIes;

  std::optional<AmfName> m_AmfName;                    // Optional
  std::optional<ServedGuamiList> m_ServedGuamiList;    // Optional
  std::optional<RelativeAmfCapacity> m_RelativeAmfCapacity;  // Optional
  std::optional<PlmnSupportList> m_PlmnSupportList;    // Optional
  // TODO: id-AMFTNLAssociationToAddList (Optional) — deferred to Stage 8
  // TODO: id-AMFTNLAssociationToRemoveList (Optional) — deferred to Stage 8
  // TODO: id-AMFTNLAssociationToUpdateList (Optional) — deferred to Stage 8
  std::optional<ExtendedAmfName> m_ExtendedAmfName;    // Optional, Rel-17
};

}  // namespace oai::ngap
#endif
