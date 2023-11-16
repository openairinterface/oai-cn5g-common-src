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

set(COMMON_SRC_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON})

include_directories(${COMMON_SRC_DIR}/config)
include_directories(${COMMON_SRC_DIR}/logger)
include_directories(${COMMON_SRC_DIR}/model/common_model)
include_directories(${COMMON_SRC_DIR}/model/udsf)

file(GLOB UDSF_MODEL_src_files
    ${COMMON_SRC_DIR}/model/common_model/AccessTokenErr.cpp
    ${COMMON_SRC_DIR}/model/common_model/AccessTokenReq.cpp
    ${COMMON_SRC_DIR}/model/common_model/Helpers.cpp
    ${COMMON_SRC_DIR}/model/common_model/InvalidParam.cpp
    ${COMMON_SRC_DIR}/model/common_model/NFType.cpp
    ${COMMON_SRC_DIR}/model/common_model/NFType_anyOf.cpp
    ${COMMON_SRC_DIR}/model/common_model/PatchItem.cpp
    ${COMMON_SRC_DIR}/model/common_model/PatchOperation.cpp
    ${COMMON_SRC_DIR}/model/common_model/PatchOperation_anyOf.cpp
    ${COMMON_SRC_DIR}/model/common_model/PatchResult.cpp
    ${COMMON_SRC_DIR}/model/common_model/PlmnId.cpp
    ${COMMON_SRC_DIR}/model/common_model/PlmnIdNid.cpp
    ${COMMON_SRC_DIR}/model/common_model/ProblemDetails.cpp
    ${COMMON_SRC_DIR}/model/common_model/ReportItem.cpp
    ${COMMON_SRC_DIR}/model/common_model/Snssai.cpp
    ${COMMON_SRC_DIR}/model/common_model/Tai.cpp      
    ${COMMON_SRC_DIR}/model/udsf/*.cpp
)
 
add_library(UDSF_MODEL STATIC ${UDSF_MODEL_src_files})