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
set(NGAP_DIR "${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap")
file(GLOB NGAP_lib_c_ies_src_files
  *.c
)
add_library (NGAP_LIB_C_IES STATIC
  ${NGAP_lib_c_ies_src_files}
)

target_include_directories(NGAP_LIB_C_IES PUBLIC ${NGAP_DIR}/libngap)
target_include_directories(NGAP_LIB_C_IES PUBLIC ${NGAP_DIR}/libngap_ies)
