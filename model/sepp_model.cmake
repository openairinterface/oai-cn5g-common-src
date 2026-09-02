# SPDX-License-Identifier: LicenseRef-CSSL-1.0

SET(SEPP_MODEL_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)

include_directories(${SEPP_MODEL_DIR})

file(GLOB SEPP_MODEL_SRC_FILES
    ${SEPP_MODEL_DIR}/AccessTokenErr.cpp
    ${SEPP_MODEL_DIR}/AccessTokenReq.cpp
    ${SEPP_MODEL_DIR}/AdditionInfoMsgForwarding.cpp
    ${SEPP_MODEL_DIR}/ApiIeMapping.cpp
    ${SEPP_MODEL_DIR}/ApiSignature.cpp
    ${SEPP_MODEL_DIR}/CallbackName.cpp
    ${SEPP_MODEL_DIR}/DataToIntegrityProtectAndCipherBlock.cpp
    ${SEPP_MODEL_DIR}/DataToIntegrityProtectBlock.cpp
    ${SEPP_MODEL_DIR}/EncodedHttpHeaderValue.cpp
    ${SEPP_MODEL_DIR}/ExtRedirectResponse.cpp
    ${SEPP_MODEL_DIR}/FailedModificationInfo.cpp
    ${SEPP_MODEL_DIR}/FailureReason.cpp
    ${SEPP_MODEL_DIR}/FlatJweJson.cpp
    ${SEPP_MODEL_DIR}/FlatJwsJson.cpp
    ${SEPP_MODEL_DIR}/Helpers.cpp
    ${SEPP_MODEL_DIR}/HttpHeader.cpp
    ${SEPP_MODEL_DIR}/HttpMethod.cpp
    ${SEPP_MODEL_DIR}/HttpMethod_anyOf.cpp
    ${SEPP_MODEL_DIR}/HttpPayload.cpp
    ${SEPP_MODEL_DIR}/IeInfo.cpp
    ${SEPP_MODEL_DIR}/IeLocation.cpp
    ${SEPP_MODEL_DIR}/IeType.cpp
    ${SEPP_MODEL_DIR}/IndexToEncryptedValue.cpp
    ${SEPP_MODEL_DIR}/IntendedN32Purpose.cpp
    ${SEPP_MODEL_DIR}/InvalidParam.cpp
    ${SEPP_MODEL_DIR}/IpxProviderSecInfo.cpp
    ${SEPP_MODEL_DIR}/MetaData.cpp
    ${SEPP_MODEL_DIR}/MlModelInterInd.cpp
    ${SEPP_MODEL_DIR}/Modifications.cpp
    ${SEPP_MODEL_DIR}/N32fContextInfo.cpp
    ${SEPP_MODEL_DIR}/N32fErrorDetail.cpp
    ${SEPP_MODEL_DIR}/N32fErrorInfo.cpp
    ${SEPP_MODEL_DIR}/N32fErrorType.cpp
    ${SEPP_MODEL_DIR}/N32fReformattedReqMsg.cpp
    ${SEPP_MODEL_DIR}/N32fReformattedRspMsg.cpp
    ${SEPP_MODEL_DIR}/N32Purpose.cpp
    ${SEPP_MODEL_DIR}/N32ReleaseIndication.cpp
    ${SEPP_MODEL_DIR}/NFType.cpp
    ${SEPP_MODEL_DIR}/NFType_anyOf.cpp
    ${SEPP_MODEL_DIR}/NoProfileMatchInfo.cpp
    ${SEPP_MODEL_DIR}/NoProfileMatchReason.cpp
    ${SEPP_MODEL_DIR}/NoProfileMatchReason_anyOf.cpp
    ${SEPP_MODEL_DIR}/NwdafEvent.cpp
    ${SEPP_MODEL_DIR}/NwdafEvent_anyOf.cpp
    ${SEPP_MODEL_DIR}/PatchItem.cpp
    ${SEPP_MODEL_DIR}/PatchOperation.cpp
    ${SEPP_MODEL_DIR}/PatchOperation_anyOf.cpp
    ${SEPP_MODEL_DIR}/PlmnId.cpp
    ${SEPP_MODEL_DIR}/PlmnIdNid.cpp
    ${SEPP_MODEL_DIR}/ProblemDetails.cpp
    ${SEPP_MODEL_DIR}/ProblemDetailsMsgForwarding.cpp
    ${SEPP_MODEL_DIR}/ProtectionPolicy.cpp
    ${SEPP_MODEL_DIR}/QueryParamCombination.cpp
    ${SEPP_MODEL_DIR}/QueryParameter.cpp
    ${SEPP_MODEL_DIR}/RedirectResponse.cpp
    ${SEPP_MODEL_DIR}/RedirectResponseAddInfo.cpp
    ${SEPP_MODEL_DIR}/RequestLine.cpp
    ${SEPP_MODEL_DIR}/RiErrorInformation.cpp
    ${SEPP_MODEL_DIR}/SecNegotiateReqData.cpp
    ${SEPP_MODEL_DIR}/SecParamExchRspData.cpp
    ${SEPP_MODEL_DIR}/SecurityCapability.cpp
    ${SEPP_MODEL_DIR}/Snssai.cpp
    ${SEPP_MODEL_DIR}/TelescopicMapping.cpp
    ${SEPP_MODEL_DIR}/Tai.cpp
    ${SEPP_MODEL_DIR}/UeContextInfoClass.cpp
    ${SEPP_MODEL_DIR}/UeContextInfoClass_anyOf.cpp
    ${SEPP_MODEL_DIR}/UriScheme.cpp
    ${SEPP_MODEL_DIR}/UriScheme_anyOf.cpp
)

if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${SEPP_MODEL_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${SEPP_MODEL_SRC_FILES}
            )
endif()
