#include "DataRefManager.hpp"

std::optional<DataRefValue> DataRefManager::get(const std::string& path)
{
	DataRef dataref = resolve(path);
    DataRefValue val;
    if(!dataref.getValue(val))
		return std::nullopt;
    return val;
}

bool DataRefManager::set(const std::string& path, DataRefValue value)
{
    DataRef dataref = resolve(path);
	return dataref.setValue(value);
}

DataRef DataRefManager::resolve(const std::string& path)
{
	if (mDataRefCache.contains(path))
    {
        return mDataRefCache[path];
    }
	DataRef dataRef(path);
	if (dataRef.isValid())
    {
        mDataRefCache[path] = dataRef;
    }
	return dataRef;
}
