# SPDX-License-Identifier: LicenseRef-CSSL-1.0

SET(AUSF_MODEL_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)

include_directories(${AUSF_MODEL_DIR})

file(GLOB AUSF_MODEL_SRC_FILES
    ${AUSF_MODEL_DIR}/AccessTech.cpp
    ${AUSF_MODEL_DIR}/AccessTech_anyOf.cpp
    ${AUSF_MODEL_DIR}/AccessTokenErr.cpp
    ${AUSF_MODEL_DIR}/AccessTokenReq.cpp
    ${AUSF_MODEL_DIR}/AuthResult.cpp
    ${AUSF_MODEL_DIR}/AuthType.cpp
    ${AUSF_MODEL_DIR}/AuthType_anyOf.cpp
    ${AUSF_MODEL_DIR}/AuthenticationInfo.cpp
    ${AUSF_MODEL_DIR}/Av5gAka.cpp
    ${AUSF_MODEL_DIR}/ConfirmationData.cpp
    ${AUSF_MODEL_DIR}/ConfirmationDataResponse.cpp
    ${AUSF_MODEL_DIR}/DeregistrationInfo.cpp
    ${AUSF_MODEL_DIR}/EapSession.cpp
    ${AUSF_MODEL_DIR}/InvalidParam.cpp
    ${AUSF_MODEL_DIR}/Ipv6Addr.cpp
    ${AUSF_MODEL_DIR}/Link.cpp
    ${AUSF_MODEL_DIR}/LinksValueSchema.cpp
    ${AUSF_MODEL_DIR}/NFType.cpp
    ${AUSF_MODEL_DIR}/NFType_anyOf.cpp
    ${AUSF_MODEL_DIR}/PatchItem.cpp
    ${AUSF_MODEL_DIR}/PatchOperation.cpp
    ${AUSF_MODEL_DIR}/PatchOperation_anyOf.cpp
    ${AUSF_MODEL_DIR}/PlmnId.cpp
    ${AUSF_MODEL_DIR}/PlmnIdNid.cpp
    ${AUSF_MODEL_DIR}/ProSeAuthData.cpp
    ${AUSF_MODEL_DIR}/ProSeAuthenticationCtx.cpp
    ${AUSF_MODEL_DIR}/ProSeAuthenticationInfo.cpp
    ${AUSF_MODEL_DIR}/ProSeAuthenticationResult.cpp
    ${AUSF_MODEL_DIR}/ProSeEapSession.cpp
    ${AUSF_MODEL_DIR}/ProblemDetails.cpp
    ${AUSF_MODEL_DIR}/RedirectResponse.cpp
    ${AUSF_MODEL_DIR}/ResynchronizationInfo.cpp
    ${AUSF_MODEL_DIR}/RgAuthCtx.cpp
    ${AUSF_MODEL_DIR}/RgAuthenticationInfo.cpp
    ${AUSF_MODEL_DIR}/ServerAddressingInfo.cpp
    ${AUSF_MODEL_DIR}/Snssai.cpp
    ${AUSF_MODEL_DIR}/SorInfo.cpp
    ${AUSF_MODEL_DIR}/SorSecurityInfo.cpp
    ${AUSF_MODEL_DIR}/SteeringContainer.cpp
    ${AUSF_MODEL_DIR}/SteeringInfo.cpp
    ${AUSF_MODEL_DIR}/Tai.cpp
    ${AUSF_MODEL_DIR}/TraceData.cpp
    ${AUSF_MODEL_DIR}/TraceDepth.cpp
    ${AUSF_MODEL_DIR}/TraceDepth_anyOf.cpp
    ${AUSF_MODEL_DIR}/UEAuthenticationCtx.cpp
    ${AUSF_MODEL_DIR}/UEAuthenticationCtx_5gAuthData.cpp
    ${AUSF_MODEL_DIR}/UeContextInfoClass.cpp
    ${AUSF_MODEL_DIR}/UeContextInfoClass_anyOf.cpp
    ${AUSF_MODEL_DIR}/UpuData.cpp
    ${AUSF_MODEL_DIR}/UpuInfo.cpp
    ${AUSF_MODEL_DIR}/UpuSecurityInfo.cpp
    ${AUSF_MODEL_DIR}/Helpers.cpp
)

if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${AUSF_MODEL_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${AUSF_MODEL_SRC_FILES}
            )
endif()
