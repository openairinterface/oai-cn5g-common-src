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
    ${AUSF_MODEL_DIR}/TraceData.cpp
    ${AUSF_MODEL_DIR}/TraceDepth.cpp
    ${AUSF_MODEL_DIR}/TraceDepth_anyOf.cpp
    ${AUSF_MODEL_DIR}/UEAuthenticationCtx.cpp
    ${AUSF_MODEL_DIR}/UEAuthenticationCtx_5gAuthData.cpp
    ${AUSF_MODEL_DIR}/UpuData.cpp
    ${AUSF_MODEL_DIR}/UpuInfo.cpp
    ${AUSF_MODEL_DIR}/UpuSecurityInfo.cpp
)

if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${AUSF_MODEL_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${AUSF_MODEL_SRC_FILES}
            )
endif()
