#pragma once

#include <string>
#include <optional>
#include "DataRefValue.hpp"

#include <nlohmann/json.hpp>

class IDataProvider {
public:
    virtual std::optional<DataRefValue> get(const std::string& path) = 0;
    virtual bool set(const std::string& path, nlohmann::json value) = 0;
    virtual ~IDataProvider() = default;
};