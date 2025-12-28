#pragma once
#include <unordered_map>
#include <DataRefs/IDataProvider.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class DummyDatarefManager : public IDataProvider
{
public:
	DummyDatarefManager();
	std::optional<DataRefValue> get(const std::string& path) override;
	bool set(const std::string& path, json value) override;
protected:
	std::unordered_map<std::string, DataRefValue> mDatarefs;
};

