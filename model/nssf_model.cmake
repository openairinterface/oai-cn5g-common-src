# SPDX-License-Identifier: LicenseRef-CSSL-1.0

SET(NSSF_MODEL_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)

include_directories(${NSSF_MODEL_DIR})

file(GLOB NSSF_MODEL_SRC_FILES
    ${NSSF_MODEL_DIR}/AccessTokenErr.cpp
    ${NSSF_MODEL_DIR}/AccessTokenReq.cpp
    ${NSSF_MODEL_DIR}/AccessType.cpp
    ${NSSF_MODEL_DIR}/AllowedNssai.cpp
    ${NSSF_MODEL_DIR}/AllowedSnssai.cpp
    ${NSSF_MODEL_DIR}/AuthorizedNetworkSliceInfo.cpp
    ${NSSF_MODEL_DIR}/AuthorizedNssaiAvailabilityData.cpp
    ${NSSF_MODEL_DIR}/AuthorizedNssaiAvailabilityInfo.cpp
    ${NSSF_MODEL_DIR}/ConfiguredSnssai.cpp
    ${NSSF_MODEL_DIR}/ExtSnssai.cpp
    ${NSSF_MODEL_DIR}/Helpers.cpp
    ${NSSF_MODEL_DIR}/InvalidParam.cpp
    ${NSSF_MODEL_DIR}/MappingOfSnssai.cpp
    ${NSSF_MODEL_DIR}/NFType.cpp
    ${NSSF_MODEL_DIR}/NFType_anyOf.cpp
    ${NSSF_MODEL_DIR}/NsagInfo.cpp
    ${NSSF_MODEL_DIR}/NsiInformation.cpp
    ${NSSF_MODEL_DIR}/NssaiAvailabilityInfo.cpp
    ${NSSF_MODEL_DIR}/NssfEventSubscriptionCreateData.cpp
    ${NSSF_MODEL_DIR}/NssfEventSubscriptionCreatedData.cpp
    ${NSSF_MODEL_DIR}/NssfEventType.cpp
    ${NSSF_MODEL_DIR}/NssfEventType_anyOf.cpp
    ${NSSF_MODEL_DIR}/PatchItem.cpp
    ${NSSF_MODEL_DIR}/PatchOperation.cpp
    ${NSSF_MODEL_DIR}/PatchOperation_anyOf.cpp
    ${NSSF_MODEL_DIR}/PlmnId.cpp
    ${NSSF_MODEL_DIR}/PlmnIdNid.cpp
    ${NSSF_MODEL_DIR}/ProblemDetails.cpp
    ${NSSF_MODEL_DIR}/RedirectResponse.cpp
    ${NSSF_MODEL_DIR}/RestrictedSnssai.cpp
    ${NSSF_MODEL_DIR}/RoamingIndication.cpp
    ${NSSF_MODEL_DIR}/RoamingIndication_anyOf.cpp
    ${NSSF_MODEL_DIR}/SdRange.cpp
    ${NSSF_MODEL_DIR}/SliceInfoForPDUSession.cpp
    ${NSSF_MODEL_DIR}/SliceInfoForRegistration.cpp
    ${NSSF_MODEL_DIR}/SliceInfoForUEConfigurationUpdate.cpp
    ${NSSF_MODEL_DIR}/Snssai.cpp
    ${NSSF_MODEL_DIR}/SnssaiInfo.cpp
    ${NSSF_MODEL_DIR}/SubscribedSnssai.cpp
    ${NSSF_MODEL_DIR}/SupportedNssaiAvailabilityData.cpp
    ${NSSF_MODEL_DIR}/TacRange.cpp
    ${NSSF_MODEL_DIR}/Tai.cpp
    ${NSSF_MODEL_DIR}/TaiRange.cpp
    ${NSSF_MODEL_DIR}/UeContextInfoClass.cpp
    ${NSSF_MODEL_DIR}/UeContextInfoClass_anyOf.cpp
)

if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${NSSF_MODEL_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${NSSF_MODEL_SRC_FILES}
            )
endif()
