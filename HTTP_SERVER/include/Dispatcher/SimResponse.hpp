#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct SimResponse {
    uint64_t id;
    bool success;
    json value;
    std::string error;
};

