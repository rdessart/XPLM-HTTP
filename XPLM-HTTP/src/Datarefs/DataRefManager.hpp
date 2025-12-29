#pragma once
#include <unordered_map>
#include <string>

#include <Simulator/IDataRefManager.hpp>
#include <nlohmann/json.hpp>
#include <XPLM/XPLMDataAccess.h>
#include <Simulator/DataRefValue.hpp>
#include "DataRef.hpp"

class DataRefManager: public IDataRefManager
{
public:
    SimResponse handle(const SimRequest& request) override;
private:
    DataRef resolve(const std::string& path);
	std::unordered_map<std::string, DataRef> mDataRefCache;
};

