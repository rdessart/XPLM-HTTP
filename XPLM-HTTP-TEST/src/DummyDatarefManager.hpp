#pragma once
#include <unordered_map>
#include <DataRefs/IDataProvider.hpp>


class DummyDatarefManager : public IDataProvider
{
public:
	DummyDatarefManager();
	std::optional<DataRefValue> get(const std::string& path) override;
	bool set(const std::string& path, DataRefValue value) override;
protected:
	std::unordered_map<std::string, DataRefValue> mDatarefs;
};

