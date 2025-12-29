#include "DummyDatarefManager.hpp"
#include <Dispatcher/SimRequest.hpp>

DummyDatarefManager::DummyDatarefManager() :
	IDataRefManager(),
	mDatarefs()
{
	mDatarefs.emplace("sim/test/float", 3.14f);
	mDatarefs.emplace("sim/test/int", 3);
	mDatarefs.emplace("sim/test/double", 3.14);
}

SimResponse DummyDatarefManager::handle(const SimRequest& request)
{
    SimResponse response;
    response.id = request.id;
    const auto& drReq = std::get<DataRefRequest>(request.Payload);
    switch (drReq.Operation)
    {
    case DataRefRequest::Get:
    {
        DataRefValue val;
        response.success = true;
        response.value = 5.65;
        return response;
    }
    case DataRefRequest::Set:
        response.success = true;
        return response;
    default:
        response.error = "Operation not found!";
        response.success = false;
        return response;
    }
}
