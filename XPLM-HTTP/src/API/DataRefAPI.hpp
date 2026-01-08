#pragma once

#include <vector>
#include <unordered_map>
#include <optional>

#include <API/BaseAPI.hpp>
#include <XPLM/XPLMDataAccess.h>

#include "../Datarefs/DataRef.hpp"


enum class DataRefEndPoint {
    Get,
    Set,
    List
};


struct DataRefOperation {
    DataRefEndPoint execution;
    std::string link;
    DataRefType type = DataRefType::Undefined;
    std::optional<json> value = std::nullopt;
};

class DataRefAPI :
    public BaseAPI<DataRefOperation, HttpResponse>
{
public:
    DataRefAPI();
    std::vector<HttpEndPoint> getEndPoints() override;
    HttpResponse Handle(int endpointIdentificator, httplib::Request const req) override;
    void MainThreadHandle() override;

private:
    HttpResponse handleList();
    HttpResponse handleGet(httplib::Request const req, uint64_t const actionId);
    HttpResponse handleSet(httplib::Request const req, uint64_t const actionId);
    
private:
    std::unordered_map<std::string, DataRef> mCache = {};
};

