/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#ifndef FILE_COMMON_ROOT_TYPES_SEEN
#define FILE_COMMON_ROOT_TYPES_SEEN

#include <stdint.h>
#include <inttypes.h>
#include <arpa/inet.h>

//------------------------------------------------------------------------------
// TEIDs
typedef uint32_t teid_t;
#define TEID_FMT "0x%" PRIx32
#define TEID_SCAN_FMT SCNx32
#define INVALID_TEID ((teid_t) 0x00000000)
#define UNASSIGNED_TEID ((teid_t) 0x00000000)

// SEIDs
typedef uint64_t seid_t;
#define SEID_FMT "0x%" PRIx64
#define SEID_SCAN_FMT SCNx64
#define INVALID_SEID ((seid_t) 0x00000000)
#define UNASSIGNED_SEID ((seid_t) 0x00000000)

//------------------------------------------------------------------------------
// IMSI
typedef uint64_t imsi64_t;
#define IMSI_64_FMT "%" SCNu64
#define INVALID_IMSI64 (imsi64_t) 0

#endif /* FILE_COMMON_ROOT_TYPES_SEEN */
