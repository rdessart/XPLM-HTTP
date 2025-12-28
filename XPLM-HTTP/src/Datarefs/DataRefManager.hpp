#pragma once
#include <unordered_map>
#include <string>
#include <DataRefs/IDataProvider.hpp>
#include <nlohmann/json.hpp>
#include <XPLM/XPLMDataAccess.h>
#include <DataRefs/DataRefValue.hpp>
#include "DataRef.hpp"

class DataRefManager :
    public IDataProvider
{
public:
    std::optional<DataRefValue> get(const std::string& path) override;
    bool set(const std::string& path, nlohmann::json value)  override;
private:
    DataRef resolve(const std::string& path);
	std::unordered_map<std::string, DataRef> mDataRefCache;
};

