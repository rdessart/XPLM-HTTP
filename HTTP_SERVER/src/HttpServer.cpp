#include <spdlog/sinks/rotating_file_sink.h>
#include "../include/HttpServer.hpp"
#include <nlohmann/json.hpp>


using json = nlohmann::json;


HttpServer::HttpServer(RequestRegistry& registry, ThreadSafeQueue<SimRequest>& requestQueue):
	mIp("127.0.0.1"),
	mPort(8080),
	mRequestQueue(requestQueue),
	mRegistry(registry),
	mRunning(false),
	mRequestId(0)
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
		
		spdlog::info("Processing DataRef GET for link: {} [id={}]", req.get_param_value("link"), mRequestId.load());
		DataRefType type = DataRefType::Undefined;
		if (req.get_param_value("type").empty() == false) {
			type = DataRefTypeFromString(req.get_param_value("type"));
			spdlog::info("[id={}] DataRef type specified: {}", mRequestId.load(), req.get_param_value("type"));
		}

		SimRequest request
		{
			generateId(),
			RequestModule::DataRef,
			DataRefRequest
			{
				DataRefRequest::Get,
				req.get_param_value("link"),
				type,
				std::nullopt
			}
		};
		sendResponse(request, res);
	});

	mServer.Post("/DataRef", [&](auto& req, auto& res) {
		json j = json::parse(req.body, nullptr, false);
		if (!j.contains("Link") || !j.contains("Value"))
		{
			res.status = 400;
			return;
		}
			
		DataRefType type = DataRefType::Undefined;
		if (req.get_param_value("type").empty() == false) {
			type = DataRefTypeFromString(req.get_param_value("type"));
			spdlog::info("[id={}] DataRef type specified: {}", mRequestId.load(), req.get_param_value("type"));
		}
		SimRequest request
		{
			generateId(),
			RequestModule::DataRef,
			DataRefRequest
			{
				DataRefRequest::Set,
				j["Link"],
				type,
				j["Value"]
			}
		};
		sendResponse(request, res);
	});

	mServer.Get("/Command/", [&](auto& req, auto& res) {
		std::optional<CommandMode> mode = StringToCommand(req.get_param_value("mode"));
		if (!mode.has_value())
		{
			res.status = 400;
			res.set_content("Missing 'mode' parameter can be once/begin/end", "text/plain");
			spdlog::critical("Request was missing mandatory 'mode' arguments...");
			return;
		}
		if (req.get_param_value("link").empty()) {
			res.status = 400;
			res.set_content("Missing 'link' parameter", "text/plain");
			spdlog::critical("Request was missing mandatory 'link' arguments...");
			return;
		}
		SimRequest request{
			generateId(),
			RequestModule::Command,
			CommandRequest{
				mode.value(),
				req.get_param_value("link")
			}
		};
		sendResponse(request, res);
	});
		
	mServer.listen(mIp.c_str(), mPort);
}

void HttpServer::sendResponse(SimRequest const request, httplib::Response& res)
{
	auto future = mRegistry.create(request.id);
	mRequestQueue.push(request);

	if (future.wait_for(DefaultTimeout) != std::future_status::ready)
	{
		res.status = 504;
		res.set_content("Timeout reached with request", "text/plain");
		return;
	}

	SimResponse simRes = future.get();
	if (!simRes.success)
	{
		res.status = 500;
		res.set_content(simRes.error, "text/plain");
	}
	res.set_content(simRes.value.dump(), "application/json");
}

//void HttpServer::sendResponse(uint64_t id, httplib::Response& res)
//{
//	SimResponse response;
//	while (mRunning) {
//		mResponseQueue.wait_pop(response);
//		if (response.id == id) {
//			json out;
//			out["ok"] = response.success;
//			if (response.success)
//				out["value"] = response.value;
//			else
//				out["error"] = response.error;
//
//			res.set_content(out.dump(), "application/json");
//			return;
//		}
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//	}
//}
