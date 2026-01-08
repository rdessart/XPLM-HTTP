#pragma once
#include <API/BaseAPI.hpp>
#include <API/HttpResponse.hpp>

#include <unordered_map>
#include "../Datarefs/DataRef.hpp"

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
	std::unordered_map<std::string, DataRef> mDataRefs = {};
};

