#include "DataRefManager.hpp"

SimResponse DataRefManager::handle(const SimRequest& request)
{
    SimResponse response;
    response.id = request.id;
    const auto& drReq = std::get<DataRefRequest>(request.Payload);
    DataRef dr = resolve(drReq.Link);
    if (!dr.isValid())
    {
        response.error = "DataRef is not found!";
        response.success = false;
        return response;
    }
    switch (drReq.Operation)
    {
    case DataRefRequest::Get:
    {
        DataRefValue val;
        if (!dr.getValue(val))
        {
            response.success = false;
            response.error = "Unable to find dataref";
            return response;
        }
        response.success = true;
        response.value = DataRefValueToJson(val);
        return response;
    }
    case DataRefRequest::Set:
        response.success = dr.setValue(drReq.Value);
        return response;
    default:
        response.error = "Operation not found!";
        response.success = false;
        return response;
    }
}


DataRef DataRefManager::resolve(const std::string& path)
{
	if (mDataRefCache.contains(path))
    {
        return mDataRefCache.at(path);
    }
	DataRef dataRef(path);
	if (dataRef.isValid())
    {
        mDataRefCache.insert(std::make_pair(path, dataRef));
    }
	return dataRef;
}
