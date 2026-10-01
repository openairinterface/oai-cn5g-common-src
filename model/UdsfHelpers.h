#ifndef Udsf_Helpers_H_
#define Udsf_Helpers_H_

#include <ctime>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "ClientId.h"
#include "Helpers.h"
#include "NFType.h"
#include "RetrieveRecords.h"
#include "SearchExpression.h"
using namespace oai::model::udsf;

namespace oai::_3gpp::model::helpers {
bool fromStringValue(
    const std::string& inStr, oai::model::udsf::ClientId& value);
bool fromStringValue(
    const std::string& inStr, oai::model::udsf::SearchExpression& value);
bool fromStringValue(
    const std::string& inStr, oai::model::udsf::ClientId& value);
bool fromStringValue(
    const std::string& inStr, oai::model::udsf::SearchExpression& value);
bool fromStringValue(
    const std::string& inStr, oai::model::udsf::RetrieveRecords& value);
}  // namespace oai::_3gpp::model::helpers

#endif  // Helpers_H_
