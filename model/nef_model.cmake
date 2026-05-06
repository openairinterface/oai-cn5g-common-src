# SPDX-License-Identifier: LicenseRef-CSSL-1.0
#
# NEF Model cmake — includes all model .cpp files from the flat model directory
# except for the 7 files that transitively depend on msg_pfcp.hpp (PFCP library
# not available in the NEF build context).
# Note: file(GLOB) is used without RECURSE so only the flat model directory
# is matched, avoiding any subdirectory model variants.

SET(NEF_MODEL_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)

include_directories(${NEF_MODEL_DIR})

file(GLOB NEF_MODEL_SRC_FILES ${NEF_MODEL_DIR}/*.cpp)

# Exclude files that transitively include msg_pfcp.hpp via UsageReport.h / SmfEventNotification.h
# (PFCP library header is not available in the NEF build)
list(REMOVE_ITEM NEF_MODEL_SRC_FILES
    ${NEF_MODEL_DIR}/AccessAndMobilitySubscriptionData.cpp
    ${NEF_MODEL_DIR}/AccuUsageReport.cpp
    ${NEF_MODEL_DIR}/ChargingDataRequest.cpp
    ${NEF_MODEL_DIR}/ChargingDataResponse.cpp
    ${NEF_MODEL_DIR}/HsmfUpdateData.cpp
    ${NEF_MODEL_DIR}/ModifyPduSession_200_response.cpp
    ${NEF_MODEL_DIR}/NsmfEventExposure.cpp
    ${NEF_MODEL_DIR}/NsmfEventExposureNotification.cpp
    ${NEF_MODEL_DIR}/PDUSessionChargingInformation.cpp
    ${NEF_MODEL_DIR}/PduSessionCreateData.cpp
    ${NEF_MODEL_DIR}/PlmnRestriction.cpp
    ${NEF_MODEL_DIR}/QosFlowsUsageReport.cpp
    ${NEF_MODEL_DIR}/QosFlowUsage_anyOf.cpp
    ${NEF_MODEL_DIR}/QosFlowUsage.cpp
    ${NEF_MODEL_DIR}/QosFlowUsageReport.cpp
    ${NEF_MODEL_DIR}/RANSecondaryRATUsageReport.cpp
    ${NEF_MODEL_DIR}/ReleaseData.cpp
    ${NEF_MODEL_DIR}/ReleasePduSession_request.cpp
    ${NEF_MODEL_DIR}/ReleaseSmContext_request.cpp
    ${NEF_MODEL_DIR}/SecondaryRatUsageInfo.cpp
    ${NEF_MODEL_DIR}/SecondaryRatUsageReport.cpp
    ${NEF_MODEL_DIR}/SmContextReleaseData.cpp
    ${NEF_MODEL_DIR}/SmContextReleaseMessage.cpp
    ${NEF_MODEL_DIR}/SmfEventNotification.cpp
    ${NEF_MODEL_DIR}/SmPolicyContextData.cpp
    ${NEF_MODEL_DIR}/SmPolicyDecision.cpp
    ${NEF_MODEL_DIR}/SmPolicyDeleteData.cpp
    ${NEF_MODEL_DIR}/SmPolicyUpdateContextData.cpp
    ${NEF_MODEL_DIR}/UeContext.cpp
    ${NEF_MODEL_DIR}/UpdatePduSession_request.cpp
    ${NEF_MODEL_DIR}/UsageReport.cpp
    ${NEF_MODEL_DIR}/VsmfUpdatedData.cpp
)

if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${NEF_MODEL_DIR})
    target_sources(${NF_TARGET} PRIVATE
        ${NEF_MODEL_SRC_FILES}
    )
endif()
