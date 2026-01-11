#pragma once
#include <API/BaseAPI.hpp>
#include <API/HttpResponse.hpp>

#include <unordered_map>
#include "../Datarefs/DataRef.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct PositionOps {

};

class PositionAPI : public BaseAPI<PositionOps, HttpResponse>
{
public:
	PositionAPI();
	std::vector<HttpEndPoint> getEndPoints() override;
	HttpResponse Handle(int requestId, httplib::Request const req) override;
	void MainThreadHandle() override;
private:
	void addDataref(const std::string& name, const std::string& path, DataRefType type);
	std::unordered_map<std::string, std::pair<std::string, DataRefType>> mDataRefPromise = {};
	std::unordered_map<std::string, std::optional<DataRef>> mDataRefs = {};
	std::unordered_map<std::string, json> mDataRefsValue = {};
};

