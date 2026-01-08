#pragma once
#include <string>
#include <functional>
#include <httplib.h>
#include "HttpResponse.hpp"

enum class HttpMethods {
	GET,
	POST,
};

struct HttpEndPoint {
	std::string Address;
	HttpMethods Methods;
	std::function<HttpResponse(int, httplib::Request)> CallBack;
	int callbackArgs;
	httplib::Request Request;
};