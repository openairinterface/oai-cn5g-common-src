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

/*! \file http_client.hpp
 \brief
 \author  Tien-Thinh NGUYEN, Stefan Spettel
 \company Eurecom, phine.tech
 \date 2023
 \email: Tien-Thinh.Nguyen@eurecom.fr, stefan.spettel@phine.tech
 */

#pragma once

#include <curl/curl.h>
#include <string>
#include <mutex>
#include <unordered_map>
#include <future>
#include <cpr/cpr.h>
#include "http_definitions.hpp"
#include "logger_base.hpp"

namespace oai::http {

    const std::string CURL_MIME_BOUNDARY = "----Boundary";
    const unsigned int HTTP_TIMEOUT_MS = 10000;

    class http_client_iface {
    public:
        /**
         * Sends an HTTP POST request
         * @param request request object
         * @return response response object
         */
        virtual response send_post(const request &request) = 0;

        /**
         * Sends an HTTP GET request
         * @param request request object
         * @return response response object
         */
        virtual response send_get(const request &request) = 0;

        /**
         * Sends an HTTP PUT request
         * @param request request object
         * @return response response object
         */
        virtual response send_put(const request &request) = 0;

        /**
         * Sends an HTTP PATCH request
         * @param request request object
         * @return response response object
         */
        virtual response send_patch(const request &request) = 0;

        /**
         * Sends an HTTP DELETE request
         * @param request request object
         * @return response response object
         */
        virtual response send_delete(const request &request) = 0;

        virtual response send_http_request(const method_e &method, const request &request) = 0;


        /**
         * Sets the correct headers for a JSON request
         * @param uri URI to send the request to
         * @param body JSON body
         * @return request object
         */
        static request prepare_json_request(
                const std::string &uri, const std::string& body);

        /**
         * Sets the correct headers for a multipart/related request
         * @param uri URI to send the request to
         * @param body multipart/related body (including boundaries)
         * @return request object
         */
        static request prepare_multipart_request(
                const std::string &uri, const std::string &body);
    };

    class http_client : public http_client_iface {
    private:
        /**
         * Sends a synchronous HTTP request (calls send_http_request_async) and waits
         * @param method
         * @param request
         * @return
         */
        response send_http_request(const method_e &method, const request &request);

        /**
         * Sends an asynchronous HTTP request in a new thread. Non-blocking.
         * Adds new CURL connections to the connection map or re-uses existing for
         * same hosts
         *
         * Note: Can also later be exposed to the API so that the
         * applications can directly send asynch HTTP requests and handle it
         * themselves
         * @param method method to use for HTTP request
         * @param request request object
         * @return future for the response object
         */
        std::future<response> send_http_request_async(
                const method_e &method, const request &request);


        response execute_http_request(const std::shared_ptr<cpr::MultiPerform> &multiPerform);


        oai::logger::printf_logger m_sbi_logger;
        int m_timeout_ms;
        std::string m_interface;
        uint8_t m_http_version;

        inline static std::shared_ptr<http_client_iface> instance;
        std::shared_ptr<cpr::MultiPerform> m_multiPerform;


    public:
        explicit http_client(
                oai::logger::printf_logger logger, int timeout_ms,
                const std::string &interface, uint8_t http_version);

        ~http_client();

        http_client(http_client const &) = delete;

        response send_post(const request &request) override;

        response send_get(const request &request) override;

        response send_put(const request &request) override;

        response send_patch(const request &request) override;

        response send_delete(const request &request) override;

        static void create_instance(
                const oai::logger::printf_logger &logger, int timeout_ms,
                const std::string &interface, uint8_t http_version);

        static std::shared_ptr<http_client_iface> get_instance();

    };

}  // namespace oai::http
