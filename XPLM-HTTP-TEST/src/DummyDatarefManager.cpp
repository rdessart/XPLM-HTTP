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

bool DummyDatarefManager::set(const std::string& path, DataRefValue value)
{
	if (mDatarefs.contains(path))
	{
		mDatarefs[path] = value;
	}

	mDatarefs.emplace(path, std::get<double>(value));
	return true;
}
