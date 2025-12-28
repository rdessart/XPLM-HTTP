#include "DummyDatarefManager.hpp"

DummyDatarefManager::DummyDatarefManager() :
	IDataProvider(),
	mDatarefs()
{
	mDatarefs.emplace("sim/test/float", 3.14f);
	mDatarefs.emplace("sim/test/int", 3);
	mDatarefs.emplace("sim/test/double", 3.14);
}

std::optional<DataRefValue> DummyDatarefManager::get(const std::string& path)
{
	if (mDatarefs.contains(path))
	{
		return mDatarefs[path];
	}

	return std::nullopt;
}

bool DummyDatarefManager::set(const std::string& path, json value)
{
	if (mDatarefs.contains(path))
	{
		mDatarefs[path] = value.get<double>();
		return true;
	}

	mDatarefs.emplace(path, value.get<double>());
	return true;
}
