#pragma once
#include <IDataProvider.hpp>
#include <XPLM/XPLMDataAccess.h>
#include <string>
#include <unordered_map>
#include <DataRefValue.hpp>
#include "DataRef.h"

class DataRefManager :
    public IDataProvider
{
public:
    std::optional<DataRefValue> get(const std::string& path) override;
    bool set(const std::string& path, DataRefValue value) override;
private:
    DataRef resolve(const std::string& path);
	std::unordered_map<std::string, DataRef> mDataRefCache;
};

