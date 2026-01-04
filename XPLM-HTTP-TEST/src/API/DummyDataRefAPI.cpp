#include "DummyDataRefAPI.hpp"

#include <spdlog/sinks/rotating_file_sink.h>

DummyDataRefAPI::DummyDataRefAPI():
    BaseAPI("DataRef")
{
}

std::vector<HttpEndPoint> DummyDataRefAPI::getEndPoints()
{
    std::vector<HttpEndPoint> endpoint;

    addEndpoint(endpoint, "", HttpMethods::GET, (int)DataRefAction::GetDataref);
    addEndpoint(endpoint, "", HttpMethods::POST, (int)DataRefAction::SetDataref);

    return endpoint;
}

HttpResponse DummyDataRefAPI::Handle(int requestId, httplib::Request const req)
{
    //1 - let's generate an unique request ID per API to help with logging
    uint64_t actionId = generateId();
    spdlog::info("New Operation with id : {}", actionId);

    //2 - Check parameters availability and generate TReq
    if (req.get_param_value("link").empty()) {
        spdlog::critical("[{}] : MISSING LINK PARAMETERS", actionId);
        return HttpResponse
        {
            .isSuccess = false,
            .statusMessage = "error:missing link",
            .returnMessage{
                {"Message", "Missing 'link' mandatory parameter"}
            }
        };
    }

    DataRefOperation ops;
    switch ((DataRefAction)requestId)
    {
    case DataRefAction::GetDataref:
        ops = GetDataref(actionId, req);
        break;
    case DataRefAction::SetDataref:
        ops = SetDataref(actionId, req);
        break;
    default:
        break;
    }

    //3 - Wait for result and timout management 
    //TODO: Check if we can put this code into the base class
    std::future<HttpResponse> resp = send_request(actionId, ops);
    if (resp.wait_for(DefaultTimeout) == std::future_status::ready)
    {
        return resp.get();
    }
    spdlog::critical("[{}] : HAS TIMED OUT", actionId);

    return HttpResponse{
        .isSuccess = false,
        .statusMessage = "timeout",
        .returnMessage{
            {"Message", "Request has timed out (>500ms)"}
        }
    };
}


DataRefOperation DummyDataRefAPI::GetDataref(uint64_t id, httplib::Request const req)
{
    spdlog::info("[{}]: is GET action", id);
    
    DataRefOperation action{
        .action = DataRefAction::GetDataref,
        .link = req.get_param_value("link"),
        .value = std::nullopt
    };
    return action;
}

DataRefOperation DummyDataRefAPI::SetDataref(uint64_t id, httplib::Request const req)
{
    spdlog::info("[{}]: is SET action", id);
    DataRefOperation action{
        .action = DataRefAction::SetDataref,
        .link = req.get_param_value("link"),
        .value = std::nullopt
    };
    return action;
}

void DummyDataRefAPI::MainThreadHandle()
{
    // 1- Get the data
    uint64_t id;
    DataRefOperation data;
    HttpResponse response;
    bool res = GetDataMainThread(id, data);
    if (!res) {
        spdlog::critical("An unexpected error has occured while getting data for thread execution");
        return;
    }
    //2 - Treat the data
    switch (data.action)
    {
    case DataRefAction::GetDataref:
        response.isSuccess = true;
        response.statusMessage = "Ok";
        response.returnMessage = {
            {"Action", "GET"},
            { "Value", 1.23456 }
        };
        break;
    case DataRefAction::SetDataref:
        response.isSuccess = true;
        response.statusMessage = "Ok";
        response.returnMessage = {
            {"Action", "SET"},
            { "Value", 1.23456 }
        };
        break;
    }
    SetReturnMainThread(id, response);
}
