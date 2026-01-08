#include "PositionAPI.h"

PositionAPI::PositionAPI() : BaseAPI("Position")
{
}

std::vector<HttpEndPoint> PositionAPI::getEndPoints()
{
	return std::vector<HttpEndPoint>();
}

HttpResponse PositionAPI::Handle(int requestId, httplib::Request const req)
{
	return HttpResponse();
}

void PositionAPI::MainThreadHandle()
{
}
