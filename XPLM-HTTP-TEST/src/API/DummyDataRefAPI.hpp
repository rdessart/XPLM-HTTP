#pragma once

#include <API/BaseAPI.hpp>

enum class DataRefAction {
	GetDataref,
	SetDataref
};

struct DataRefOperation {
	DataRefAction action;
	std::string link;
	std::optional<std::string> value;
};

class DummyDataRefAPI : public BaseAPI<DataRefOperation, HttpResponse>
{
public:
	DummyDataRefAPI();

	std::vector<HttpEndPoint> getEndPoints() override;
	HttpResponse Handle(int requestId, httplib::Request const req) override;
	void MainThreadHandle() override;

private:
	DataRefOperation GetDataref(uint64_t id, httplib::Request const req);
	DataRefOperation SetDataref(uint64_t id, httplib::Request const req);
};

