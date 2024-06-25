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

/*! \file spd_logger_base.hpp
\brief
\author Stefan Spettel
\company OpenAirInterface Software Alliance
\date 2022
\email: stefan.spettel@eurecom.fr
*/

#pragma once

#include <cstdarg>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

// Used by SPDLOG to use external FMT library
#define SPDLOG_FMT_EXTERNAL

// this way we redefine: warn->start, error->warn, critical->error
#define SPDLOG_LEVEL_NAMES                                                     \
  {"trace", "debug", "info", "start", "warning", "error", "off"};

#include <fmt/printf.h>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

namespace oai::logger {
/**
 * Wrapper class for spdlog, stores reference to spdlog log object and allows
 * safe printf-style formatting
 */
class spd_logger {
 public:
  spd_logger(
      const std::string& nf_name, const std::string& name, bool log_stdout,
      bool log_rot_file);

  void set_level(spdlog::level::level_enum level);
  bool should_log(spdlog::level::level_enum level) {
    return logger->should_log(level);
  }

  template<typename... T>
  void log_printf(
      const spdlog::level::level_enum& lvl, const std::string& fmt,
      const T&... args) const {
    // to prevent "expensive" string formatting
    if (!logger->should_log(lvl)) {
      return;
    }

    try {
      std::string format = fmt::sprintf(fmt, args...);
      logger->log(lvl, "{}", format);
    } catch (fmt::format_error& err) {
      // It would be better to not catch here, but keep it here for now
      // to ensure that we don't break when we replace the logger
      logger->error("Format error in format string {}: {}", fmt, err.what());
    } catch (std::exception& e) {
      logger->error("Format error in format string {}", e.what());
    }
  }

 private:
  std::shared_ptr<spdlog::logger> logger;
};
}  // namespace oai::logger
