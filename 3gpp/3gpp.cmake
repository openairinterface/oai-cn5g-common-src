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

SET(3GPP_DIR ${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp)
include_directories(${3GPP_DIR})

file(GLOB 3GPP_SRC_FILES
        ${3GPP_DIR}/*.cpp
        )

if (TARGET ${NF_TARGET})
target_include_directories(${NF_TARGET} PUBLIC ${3GPP_DIR})
target_sources(${NF_TARGET} PRIVATE
        ${3GPP_SRC_FILES}
        )
endif()
