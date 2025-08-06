################################################################################
# Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The OpenAirInterface Software Alliance licenses this file to You under
# the OAI Public License, Version 1.1  (the "License"); you may not use this file
# except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.openairinterface.org/?page_id=698
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#-------------------------------------------------------------------------------
# For more information about the OpenAirInterface (OAI) Software Alliance:
#      contact@openairinterface.org
################################################################################

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/common_model.cmake)
include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/nrf_model.cmake)

SET(AMF_MODEL_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)

set(COMMON_MODEL_SRC_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)

include_directories(${AMF_MODEL_DIR})

file(GLOB AMF_MODEL_SRC_FILES
        ${COMMON_MODEL_DIR}/_5GMmCapability.cpp
        ${COMMON_MODEL_DIR}/_5GsUserState_anyOf.cpp
        ${COMMON_MODEL_DIR}/5GsUserState.cpp
        ${COMMON_MODEL_DIR}/5GsUserStateInfo.cpp
        ${COMMON_MODEL_DIR}/AllowedNssai.cpp
        ${COMMON_MODEL_DIR}/AllowedSnssai.cpp
        ${COMMON_MODEL_DIR}/AmfCreatedEventSubscription.cpp
        ${COMMON_MODEL_DIR}/AmfCreateEventSubscription.cpp
        ${COMMON_MODEL_DIR}/AmfEventArea.cpp
        ${COMMON_MODEL_DIR}/AmfEvent.cpp
        ${COMMON_MODEL_DIR}/AmfEventMode.cpp
        ${COMMON_MODEL_DIR}/AmfEventNotification.cpp
        ${COMMON_MODEL_DIR}/AmfEventReport.cpp
        ${COMMON_MODEL_DIR}/AmfEventState.cpp
        ${COMMON_MODEL_DIR}/AmfEventSubscription.cpp
        ${COMMON_MODEL_DIR}/AmfEventTrigger_anyOf.cpp
        ${COMMON_MODEL_DIR}/AmfEventTrigger.cpp
        ${COMMON_MODEL_DIR}/AmfEventType_anyOf.cpp
        ${COMMON_MODEL_DIR}/AmfEventType.cpp
        ${COMMON_MODEL_DIR}/AmfStatusChangeNotification.cpp
        ${COMMON_MODEL_DIR}/AmfStatusInfo.cpp
        ${COMMON_MODEL_DIR}/AmfUpdatedEventSubscription.cpp
        ${COMMON_MODEL_DIR}/AmfUpdateEventOptionItem.cpp
        ${COMMON_MODEL_DIR}/AreaOfValidity.cpp
        ${COMMON_MODEL_DIR}/AssignEbiData.cpp
        ${COMMON_MODEL_DIR}/AssignEbiError.cpp
        ${COMMON_MODEL_DIR}/AssignEbiFailed.cpp
        ${COMMON_MODEL_DIR}/AssignedEbiData.cpp
        ${COMMON_MODEL_DIR}/AuthenticationInfo.cpp
        ${COMMON_MODEL_DIR}/AuthorizedNetworkSliceInfo.cpp
        ${COMMON_MODEL_DIR}/AuthResult.cpp
        ${COMMON_MODEL_DIR}/Av5gAka.cpp
        ${COMMON_MODEL_DIR}/Cause.cpp
        ${COMMON_MODEL_DIR}/CipheringAlgorithm.cpp
        ${COMMON_MODEL_DIR}/CmInfo.cpp
        ${COMMON_MODEL_DIR}/CmState_anyOf.cpp
        ${COMMON_MODEL_DIR}/CmState.cpp
        ${COMMON_MODEL_DIR}/CommunicationFailure.cpp
        ${COMMON_MODEL_DIR}/ConfiguredSnssai.cpp
        ${COMMON_MODEL_DIR}/ConfirmationData.cpp
        ${COMMON_MODEL_DIR}/ConfirmationDataResponse.cpp
        ${COMMON_MODEL_DIR}/DrxParameter.cpp
        ${COMMON_MODEL_DIR}/EbiArpMapping_2.cpp
        ${COMMON_MODEL_DIR}/EbiArpMapping.cpp
        ${COMMON_MODEL_DIR}/ExpectedUeBehavior.cpp
        ${COMMON_MODEL_DIR}/Inline_object.cpp
        ${COMMON_MODEL_DIR}/IntegrityAlgorithm.cpp
        ${COMMON_MODEL_DIR}/InvalidParam_2.cpp
        ${COMMON_MODEL_DIR}/KeyAmf.cpp
        ${COMMON_MODEL_DIR}/KeyAmfType.cpp
        ${COMMON_MODEL_DIR}/LadnInfo.cpp
        ${COMMON_MODEL_DIR}/LocationFilter_anyOf.cpp
        ${COMMON_MODEL_DIR}/LocationFilter.cpp
        ${COMMON_MODEL_DIR}/LossOfConnectivityReason_anyOf.cpp
        ${COMMON_MODEL_DIR}/LossOfConnectivityReason.cpp
        ${COMMON_MODEL_DIR}/MappingOfSnssai.cpp
        ${COMMON_MODEL_DIR}/MmContext.cpp
        ${COMMON_MODEL_DIR}/N1MessageClass_anyOf.cpp
        ${COMMON_MODEL_DIR}/N1MessageClass.cpp
        ${COMMON_MODEL_DIR}/N1MessageContainer.cpp
        ${COMMON_MODEL_DIR}/N1MessageNotification.cpp
        ${COMMON_MODEL_DIR}/N1N2MessageTransferCause.cpp
        ${COMMON_MODEL_DIR}/N1N2MessageTransferError.cpp
        ${COMMON_MODEL_DIR}/N1N2MessageTransferReqData.cpp
        ${COMMON_MODEL_DIR}/N1N2MessageTransferRspData.cpp
        ${COMMON_MODEL_DIR}/N1N2MsgTxfrErrDetail.cpp
        ${COMMON_MODEL_DIR}/N1N2MsgTxfrFailureNotification.cpp
        ${COMMON_MODEL_DIR}/N2InfoContainer.cpp
        ${COMMON_MODEL_DIR}/N2InfoContent.cpp
        ${COMMON_MODEL_DIR}/N2InfoNotificationRspData.cpp
        ${COMMON_MODEL_DIR}/N2InfoNotifyReason.cpp
        ${COMMON_MODEL_DIR}/N2InformationClass_anyOf.cpp
        ${COMMON_MODEL_DIR}/N2InformationClass.cpp
        ${COMMON_MODEL_DIR}/N2InformationNotification.cpp
        ${COMMON_MODEL_DIR}/N2InformationTransferError.cpp
        ${COMMON_MODEL_DIR}/N2InformationTransferReqData.cpp
        ${COMMON_MODEL_DIR}/N2InformationTransferResult.cpp
        ${COMMON_MODEL_DIR}/N2InformationTransferRspData.cpp
        ${COMMON_MODEL_DIR}/N2RanInformation.cpp
        ${COMMON_MODEL_DIR}/N2SmInformation.cpp
        ${COMMON_MODEL_DIR}/NasCount.cpp
        ${COMMON_MODEL_DIR}/NasSecurityMode.cpp
        ${COMMON_MODEL_DIR}/NgapIeType_anyOf.cpp
        ${COMMON_MODEL_DIR}/NgapIeType.cpp
        ${COMMON_MODEL_DIR}/NgKsi.cpp
        ${COMMON_MODEL_DIR}/NgRanTargetId.cpp
        ${COMMON_MODEL_DIR}/NonUeN2InfoSubscriptionCreateData.cpp
        ${COMMON_MODEL_DIR}/NonUeN2InfoSubscriptionCreatedData.cpp
        ${COMMON_MODEL_DIR}/NrppaInformation.cpp
        ${COMMON_MODEL_DIR}/NsiInformation.cpp
        ${COMMON_MODEL_DIR}/Nssai.cpp
        ${COMMON_MODEL_DIR}/NssaiMapping.cpp
        ${COMMON_MODEL_DIR}/PduSessionContext.cpp
        ${COMMON_MODEL_DIR}/PolicyReqTrigger.cpp
        ${COMMON_MODEL_DIR}/PWSErrorData.cpp
        ${COMMON_MODEL_DIR}/PwsInformation.cpp
        ${COMMON_MODEL_DIR}/PWSResponseData.cpp
        ${COMMON_MODEL_DIR}/RatSelector.cpp
        ${COMMON_MODEL_DIR}/RegistrationContextContainer.cpp
        ${COMMON_MODEL_DIR}/ResourceStatus.cpp
        ${COMMON_MODEL_DIR}/ResynchronizationInfo.cpp
        ${COMMON_MODEL_DIR}/RmInfo.cpp
        ${COMMON_MODEL_DIR}/RmState_anyOf.cpp
        ${COMMON_MODEL_DIR}/RmState.cpp
        ${COMMON_MODEL_DIR}/S1UeNetworkCapability.cpp
        ${COMMON_MODEL_DIR}/ScType.cpp
        ${COMMON_MODEL_DIR}/SeafData.cpp
        ${COMMON_MODEL_DIR}/ServerAddressingInfo.cpp
        ${COMMON_MODEL_DIR}/SliceInfoForRegistration.cpp
        ${COMMON_MODEL_DIR}/SmContextStatusNotification.cpp
        ${COMMON_MODEL_DIR}/SmfChangeIndication.cpp
        ${COMMON_MODEL_DIR}/SmsSupport.cpp
        ${COMMON_MODEL_DIR}/StatusChange.cpp
        ${COMMON_MODEL_DIR}/StatusInfo.cpp
        ${COMMON_MODEL_DIR}/SubscribedSnssai.cpp
        ${COMMON_MODEL_DIR}/SubscriptionData.cpp
        ${COMMON_MODEL_DIR}/TransferReason.cpp
        ${COMMON_MODEL_DIR}/UEAuthenticationCtx.cpp
        ${COMMON_MODEL_DIR}/UeContext.cpp
        ${COMMON_MODEL_DIR}/UeContextCreateData.cpp
        ${COMMON_MODEL_DIR}/UeContextCreatedData.cpp
        ${COMMON_MODEL_DIR}/UeContextCreateError.cpp
        ${COMMON_MODEL_DIR}/UEContextRelease.cpp
        ${COMMON_MODEL_DIR}/UeContextTransferReqData.cpp
        ${COMMON_MODEL_DIR}/UeContextTransferRspData.cpp
        ${COMMON_MODEL_DIR}/UeContextTransferStatus.cpp
        ${COMMON_MODEL_DIR}/UeN1N2InfoSubscriptionCreateData.cpp
        ${COMMON_MODEL_DIR}/UeN1N2InfoSubscriptionCreatedData.cpp
        ${COMMON_MODEL_DIR}/UeReachability_anyOf.cpp
        ${COMMON_MODEL_DIR}/UeReachability.cpp
        ${COMMON_MODEL_DIR}/UeRegStatusUpdateReqData.cpp
        ${COMMON_MODEL_DIR}/UeRegStatusUpdateRspData.cpp
        ${COMMON_MODEL_DIR}/UeSecurityCapability.cpp
        ${COMMON_MODEL_DIR}/V2xInformation.cpp
        ${COMMON_MODEL_DIR}/AccessTokenErr.cpp
        ${COMMON_MODEL_DIR}/AccessTokenReq.cpp
        ${COMMON_MODEL_DIR}/AccessType.cpp
        ${COMMON_MODEL_DIR}/Ambr.cpp
        ${COMMON_MODEL_DIR}/Area.cpp
        ${COMMON_MODEL_DIR}/Arp.cpp
        ${COMMON_MODEL_DIR}/CellGlobalId.cpp
        ${COMMON_MODEL_DIR}/CoreNetworkType.cpp
        ${COMMON_MODEL_DIR}/CoreNetworkType_anyOf.cpp
        ${COMMON_MODEL_DIR}/Ecgi.cpp
        ${COMMON_MODEL_DIR}/EutraLocation.cpp
        ${COMMON_MODEL_DIR}/GeraLocation.cpp
        ${COMMON_MODEL_DIR}/GNbId.cpp
        ${COMMON_MODEL_DIR}/GlobalRanNodeId.cpp
        ${COMMON_MODEL_DIR}/Guami.cpp
        ${COMMON_MODEL_DIR}/Helpers.cpp
        ${COMMON_MODEL_DIR}/HfcNodeId.cpp
        ${COMMON_MODEL_DIR}/InvalidParam.cpp
        ${COMMON_MODEL_DIR}/Ipv6Addr.cpp
        ${COMMON_MODEL_DIR}/LineType.cpp
        ${COMMON_MODEL_DIR}/LineType_anyOf.cpp
        ${COMMON_MODEL_DIR}/LinksValueSchema.cpp
        ${COMMON_MODEL_DIR}/LocationAreaId.cpp
        ${COMMON_MODEL_DIR}/N3gaLocation.cpp
        ${COMMON_MODEL_DIR}/NFType.cpp
        ${COMMON_MODEL_DIR}/NFType_anyOf.cpp
        ${COMMON_MODEL_DIR}/Ncgi.cpp
        ${COMMON_MODEL_DIR}/NgApCause.cpp
        ${COMMON_MODEL_DIR}/NrLocation.cpp
        ${COMMON_MODEL_DIR}/PatchItem.cpp
        ${COMMON_MODEL_DIR}/PatchOperation.cpp
        ${COMMON_MODEL_DIR}/PatchOperation_anyOf.cpp
        ${COMMON_MODEL_DIR}/PlmnId.cpp
        ${COMMON_MODEL_DIR}/PlmnIdNid.cpp
        ${COMMON_MODEL_DIR}/PreemptionCapability.cpp
        ${COMMON_MODEL_DIR}/PreemptionCapability_anyOf.cpp
        ${COMMON_MODEL_DIR}/PreemptionVulnerability.cpp
        ${COMMON_MODEL_DIR}/PreemptionVulnerability_anyOf.cpp
        ${COMMON_MODEL_DIR}/PresenceInfo.cpp
        ${COMMON_MODEL_DIR}/PresenceState.cpp
        ${COMMON_MODEL_DIR}/PresenceState_anyOf.cpp
        ${COMMON_MODEL_DIR}/ProblemDetails.cpp
        ${COMMON_MODEL_DIR}/RatType.cpp
        ${COMMON_MODEL_DIR}/RatType_anyOf.cpp
        ${COMMON_MODEL_DIR}/RefToBinaryData.cpp
        ${COMMON_MODEL_DIR}/RestrictionType.cpp
        ${COMMON_MODEL_DIR}/RestrictionType_anyOf.cpp
        ${COMMON_MODEL_DIR}/RoutingAreaId.cpp
        ${COMMON_MODEL_DIR}/ServiceAreaId.cpp
        ${COMMON_MODEL_DIR}/ServiceAreaRestriction.cpp
        ${COMMON_MODEL_DIR}/Snssai.cpp
        ${COMMON_MODEL_DIR}/Tai.cpp
        ${COMMON_MODEL_DIR}/TnapId.cpp
        ${COMMON_MODEL_DIR}/TraceData.cpp
        ${COMMON_MODEL_DIR}/TraceDepth.cpp
        ${COMMON_MODEL_DIR}/TraceDepth_anyOf.cpp
        ${COMMON_MODEL_DIR}/TransportProtocol.cpp
        ${COMMON_MODEL_DIR}/TransportProtocol_anyOf.cpp
        ${COMMON_MODEL_DIR}/TwapId.cpp
        ${COMMON_MODEL_DIR}/UserLocation.cpp
        ${COMMON_MODEL_DIR}/UtraLocation.cpp 
)

## CONFIG used in NF_TARGET (main)
if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${AMF_MODEL_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${AMF_MODEL_SRC_FILES}
            )
endif()
