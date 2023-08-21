################################################################################
#Licensed to the OpenAirInterface(OAI) Software Alliance under one or more
#contributor license agreements.See the NOTICE file distributed with
#this work for additional information regarding copyright ownership.
#The OpenAirInterface Software Alliance licenses this file to You under
#the OAI Public License, Version 1.1(the "License"); you may not use this file
#except in compliance with the License.
#You may obtain a copy of the License at
#
#http:  // www.openairinterface.org/?page_id=698
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
#For more information about the OpenAirInterface(OAI) Software Alliance:
#contact @openairinterface.org
################################################################################

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/common_model/common_model.cmake)

SET(NRF_MODEL_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/nrf)

include_directories(${NRF_MODEL_DIR})

file(GLOB NRF_MODEL_SRC_FILES
        ${NRF_MODEL_DIR}/*.cpp
        )

## CONFIG used in NF_TARGET (main)
if (TARGET ${NF_TARGET})
    target_include_directories(${NF_TARGET} PUBLIC ${NRF_MODEL_DIR})
    target_sources(${NF_TARGET} PRIVATE
            ${NRF_MODEL_SRC_FILES}
            )
endif()