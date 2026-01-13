#include "../../include/Server/HttpServer.hpp"

#include <iostream>
#include <sstream>

#include <spdlog/sinks/rotating_file_sink.h>

#include "../../include/API/IBaseAPI.hpp"
#include "../../include/API/HttpEndPoint.hpp"

HttpServer::HttpServer() :
	mServer(httplib::Server()),
	mRunning(false)
{
	auto max_size = 1048576 * 5;
	auto max_files = 3;
	mLogger = spdlog::rotating_logger_mt("httpServer", "log/httpServer.txt", max_size, max_files);
	spdlog::set_default_logger(mLogger);
	spdlog::flush_on(spdlog::level::trace);
}

void HttpServer::SetListeningAddress(const std::string& ip, int port)
{
	mIp = ip;
	mPort = port;
}

void HttpServer::Run()
{
	spdlog::info("Server is listening on {}", getBaseAddress());
	Init();
	bool res = mServer.listen(mIp, mPort);
	mRunning = res;
	spdlog::critical("SERVER IS UNABLE TO BIND ON {}", getBaseAddress());
	mServer.stop();
}

void HttpServer::Init()
{
	if (!mServer.set_mount_point("/", "www/"))
	{
		spdlog::warn("No 'www' directory founded! this endpoint will not be active");
	}

	mServer.Get("/API", [&](const httplib::Request& req, httplib::Response& res)
		{
			res.set_content("Hello World", "text/plain");
		});


	for (auto& kv : mApiMap)
	{
		for (auto api : kv.second.getEndPoints())
		{
			std::stringstream address;
			address << "/API/" << kv.first << api.Address;
			switch (api.Methods)
			{
			case HttpMethods::GET:
				spdlog::info("API: {} - Registering [GET] address: {}{}", kv.first, getBaseAddress(), address.str());
				mServer.Get(address.str(), [=](const httplib::Request& req, httplib::Response& res)
					{
						auto result = api.CallBack(api.callbackArgs, req);
						res.status = result.isSuccess ? 200 : 500;
						res.set_content(result.Serialize().dump(), "application/json");
					});
				break;

			case HttpMethods::POST:
				spdlog::info("API: {} - Registering [POST] address: {}{}", kv.first, getBaseAddress(), address.str());
				mServer.Post(address.str(), [=](const httplib::Request& req, httplib::Response& res)
					{
						auto result = api.CallBack(api.callbackArgs, req);
						res.status = result.isSuccess ? 200 : 500;
						res.set_content(result.Serialize().dump(), "application/json");
					});
				break;
			default:
				break;
			}
		}
	}
}

void HttpServer::RegisterApi(IBaseAPI& api)
{
	mApiMap.insert(std::pair<std::string, IBaseAPI&>(api.getName(), api));
}

void HttpServer::Start()
{
	mRunning = true;
	mThread = std::thread(&HttpServer::Run, this);
}

std::string HttpServer::getBaseAddress() const
{
	std::stringstream address;
	address << "http://" << mIp << ":" << mPort;
	return address.str();
}

void HttpServer::cors_data(std::unordered_map<std::string, std::string> const& headers)
{
	httplib::Headers headerList;
	for (auto const& [key, value] : headers)
	{
		headerList.insert({ {key, value} });
	}
	
	mServer.set_default_headers(headerList);
}
