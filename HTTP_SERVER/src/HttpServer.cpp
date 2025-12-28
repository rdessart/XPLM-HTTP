#include <spdlog/sinks/rotating_file_sink.h>
#include "../include/HttpServer.hpp"
#include <nlohmann/json.hpp>


using json = nlohmann::json;

HttpServer::HttpServer(ThreadSafeQueue<SimRequest>& requestQueue, ThreadSafeQueue<SimResponse>& responseQueue):
	mIp("127.0.0.1"),
	mPort(8080),
	mRequestQueue(requestQueue),
	mResponseQueue(responseQueue),
	mRunning(false)
{
	auto max_size = 1048576 * 5;
	auto max_files = 3;
	mLogger = spdlog::rotating_logger_mt("httpServer", "logs/httpServer.txt", max_size, max_files);
}

void HttpServer::init(int port, const std::string& address)
{
	mIp = address;
	mPort = port;
}

void HttpServer::start()
{
	mRunning = true;
	mThread = std::thread(&HttpServer::run, this);
}

void HttpServer::stop()
{
	mRunning = false;
	if (mThread.joinable())
		mThread.join();
	mServer.stop();
}

void HttpServer::run()
{
	mServer.Get("/", [](const httplib::Request& req, httplib::Response& res) {
		spdlog::info("Received request for '/'");
		res.set_content("Hello, World!", "text/plain");
		});

	mServer.Get("/DataRef", [&](auto& req, auto& res) {
		spdlog::info("Received request for '/DataRef'");
		if (req.get_param_value("link").empty()) {
			res.status = 400;
			res.set_content("Missing 'link' parameter", "text/plain");
			spdlog::critical("Request was missing mandatory 'link' arguments...");
			return;
		}
		SimRequest request;
		request.id = mRequestId++;
		request.Type = RequestModule::DataRef;
		spdlog::info("Processing DataRef GET for link: {} [id={}]", req.get_param_value("link"), mRequestId);
		DataRefType type = DataRefType::Undefined;
		if (req.get_param_value("type").empty() == false) {
			type = DataRefTypeFromString(req.get_param_value("type"));
			spdlog::info("[id={}] DataRef type specified: {}", mRequestId, req.get_param_value("type"));
		}

		request.Payload = DataRefRequest
		{
			DataRefRequest::Get,
			req.get_param_value("link"),
			type,
			std::nullopt
		};

		mRequestQueue.push(request);
		sendResponse(request.id, res);
		});

	mServer.Post("/DataRef", [&](auto& req, auto& res) {
			json j = json::parse(req.body, nullptr, false);
			if (!j.contains("Link") || !j.contains("Value"))
			{
				res.status = 400;
				return;
			}
			SimRequest request
			{
				mRequestId++,
				RequestModule::DataRef,
			};
			DataRefType type = DataRefType::Undefined;
			if (req.get_param_value("type").empty() == false) {
				type = DataRefTypeFromString(req.get_param_value("type"));
				spdlog::info("[id={}] DataRef type specified: {}", mRequestId, req.get_param_value("type"));
			}
			request.Payload = DataRefRequest
			{
				DataRefRequest::Set,
				j["Link"],
				type,
				j["Value"]
			};
			mRequestQueue.push(request);
			sendResponse(request.id, res);
		});

	mServer.Get("/Command", [&](auto& req, auto& res) {
		res.set_content("Command", "text/plain");
		});
	mServer.listen(mIp.c_str(), mPort);
}

void HttpServer::sendResponse(uint64_t id, httplib::Response& res)
{
	SimResponse response;
	while (mRunning) {
		mResponseQueue.wait_pop(response);
		if (response.id == id) {
			json out;
			out["ok"] = response.success;
			if (response.success)
				out["value"] = response.value;
			else
				out["error"] = response.error;

			res.set_content(out.dump(), "application/json");
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
