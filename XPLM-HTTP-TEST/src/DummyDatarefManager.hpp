#pragma once
#include <unordered_map>
#include <Simulator/IDataRefManager.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class DummyDatarefManager : public IDataRefManager
{
public:
	DummyDatarefManager();
	SimResponse handle(const SimRequest& request) override;
protected:
	std::unordered_map<std::string, DataRefValue> mDatarefs;
};

