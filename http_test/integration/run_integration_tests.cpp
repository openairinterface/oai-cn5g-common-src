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

/*! \file run_integration_tests.cpp
 \brief
 \author  Stefan Spettel, Luksa Rotheneder
 \company phine.tech
 \date    2023
 \email:  stefan.spettel@phine.tech, lukas.rotheneder@phine.tech
 */

#include "http_definitions.hpp"
#include "http_client.hpp"
#include <string>
#include <random>
#include <thread>
#include <chrono>

using namespace oai;
using namespace oai::http;

const int NUMBER_OF_THREADS = 100;
const int HTTP_TIMEOUT      = 4000;  // in ms
const int FUTURE_WAIT       = 2;     // in s
const bool SLEEP            = false;
const int SLEEP_DUR         = 1;  // in ms
const bool SEQ_SLEEP        = true;
const int SEQ_SLEEP_DUR     = 100;

static std::atomic<int> unsuccessful_threads(0);

void task(std::future<response>& future_resp) {
  std::future_status status =
      future_resp.wait_for(std::chrono::seconds(FUTURE_WAIT));
  if (status != std::future_status::ready) {
    logger::logger_registry::get_logger("test").info(
        "wait_for has expired! No Response");
    unsuccessful_threads++;
  } else {
    auto resp         = future_resp.get();
    std::string b_str = resp.body;
    auto stat_code    = static_cast<std::underlying_type<status_code_e>::type>(
        resp.status_code);
    logger::logger_registry::get_logger("test").trace(
        "Status Code: %u, Body: %s", stat_code, b_str);
  }
}

int main(int argc, char* argv[]) {
  logger::logger_registry::register_logger("common-src", "test", true, false);
  logger::logger_registry::set_level(spdlog::level::level_enum::trace);

  http_client::create_instance(
      logger::logger_registry::get_logger("test"), HTTP_TIMEOUT, "lo", 2);

  auto http_client = oai::http::http_client::get_instance();
  int num_threads  = NUMBER_OF_THREADS;
  auto start       = std::chrono::high_resolution_clock::now();

  std::vector<std::future<response>> responses;

  for (int i = 0; i < num_threads; i++) {
    if (SLEEP) {
      std::this_thread::sleep_for(std::chrono::milliseconds{SLEEP_DUR});
    }
    std::string ruri;
    if (i == 5) {
      ruri = "http://localhost:80/sleep";
    } else if (i == 10) {
      ruri = "http://localhost:80/sleep";
    } else {
      ruri = "http://localhost:80/post";
    }
    request r = http_client->prepare_json_request(
        ruri, std::string("{\n"
                          "    \"test\" : {\n"
                          "        \"key\":\"test\"\n"
                          "    },\n"
                          "    \"test_values\" : {\n"
                          "        \"w\" : \"x\",\n"
                          "        \"y\" : \"z\"\n"
                          "    }\n"
                          "}"));
    responses.push_back(
        std::async([http_client, r] { return http_client->send_post(r); }));
  }
  std::vector<std::thread> threads;
  for (auto& resp : responses) {
    threads.emplace_back(task, std::ref(resp));
  }
  for (auto& th : threads) {
    th.join();
  }

  auto stop_1 = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop_1 - start);
  logger::logger_registry::get_logger("test").info(
      "FIN PARALLEL - %i wait_fore were expired - DURATION: %i (ms)",
      unsuccessful_threads, duration.count());

  for (int i = 0; i < num_threads; i++) {
    request r;
    if (i == 5) {
      r.uri = "http://127.0.0.1:80/sleep";
    } else if (i == 10) {
      r.uri = "http://127.0.0.1:80/sleep";
    } else {
      r.uri = "http://127.0.0.1:80/test";
    }
    if (SEQ_SLEEP) {
      std::this_thread::sleep_for(std::chrono::milliseconds{SEQ_SLEEP_DUR});
    }

    response resp     = http_client->send_get(r);
    std::string b_str = resp.body;
    auto stat_code    = static_cast<std::underlying_type<status_code_e>::type>(
        resp.status_code);
    logger::logger_registry::get_logger("test").trace(
        "Status Code: %u, Body: %s", stat_code, b_str);
  }
  auto stop_2 = std::chrono::high_resolution_clock::now();
  auto duration_2 =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop_2 - stop_1);

  logger::logger_registry::get_logger("test").info(
      "FIN SEQUENTIAL - DURATION: %i (ms)", duration_2.count());

  return 0;
}