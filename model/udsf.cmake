# SPDX-License-Identifier: LicenseRef-CSSL-1.0
SET(UDSF_MODEL_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)

include_directories(${UDSF_MODEL_DIR})

file(GLOB UDSF_MODEL_SRC_FILES
    ${UDSF_MODEL_DIR}/AccessTokenErr.cpp
    ${UDSF_MODEL_DIR}/AccessTokenReq.cpp
    ${UDSF_MODEL_DIR}/ClientId.cpp
    ${UDSF_MODEL_DIR}/ComparisonOperator.cpp
    ${UDSF_MODEL_DIR}/ConditionOperator.cpp
    ${UDSF_MODEL_DIR}/GetBlockList_200_response.cpp
    ${UDSF_MODEL_DIR}/Helpers.cpp
    ${UDSF_MODEL_DIR}/InvalidParam.cpp
    ${UDSF_MODEL_DIR}/KeyType.cpp
    ${UDSF_MODEL_DIR}/MetaSchema.cpp
    ${UDSF_MODEL_DIR}/NFType.cpp
    ${UDSF_MODEL_DIR}/NFType_anyOf.cpp
    ${UDSF_MODEL_DIR}/NotificationSubscription.cpp
    ${UDSF_MODEL_DIR}/PatchItem.cpp
    ${UDSF_MODEL_DIR}/PatchOperation.cpp
    ${UDSF_MODEL_DIR}/PatchOperation_anyOf.cpp
    ${UDSF_MODEL_DIR}/PatchResult.cpp
    ${UDSF_MODEL_DIR}/PlmnId.cpp
    ${UDSF_MODEL_DIR}/PlmnIdNid.cpp
    ${UDSF_MODEL_DIR}/ProblemDetails.cpp
    ${UDSF_MODEL_DIR}/Record.cpp
    ${UDSF_MODEL_DIR}/RecordIdList.cpp
    ${UDSF_MODEL_DIR}/RecordMeta.cpp
    ${UDSF_MODEL_DIR}/RecordOperation.cpp
    ${UDSF_MODEL_DIR}/RecordSearchResult.cpp
    ${UDSF_MODEL_DIR}/ReportItem.cpp
    ${UDSF_MODEL_DIR}/RetrieveRecords.cpp
    ${UDSF_MODEL_DIR}/SearchComparison.cpp
    ${UDSF_MODEL_DIR}/SearchCondition.cpp
    ${UDSF_MODEL_DIR}/SearchExpression.cpp
    ${UDSF_MODEL_DIR}/Snssai.cpp
    ${UDSF_MODEL_DIR}/SubscriptionFilter.cpp
    ${UDSF_MODEL_DIR}/Tai.cpp    
    ${UDSF_MODEL_DIR}/TagType.cpp
    ${UDSF_MODEL_DIR}/UdsfHelpers.cpp
    ${UDSF_MODEL_DIR}/UeContextInfoClass.cpp
    ${UDSF_MODEL_DIR}/UeContextInfoClass_anyOf.cpp
)

## CONFIG used in NF_TARGET (main)
if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${UDSF_MODEL_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${UDSF_MODEL_SRC_FILES}
            )
endif()
