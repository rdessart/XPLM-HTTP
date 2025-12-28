#include "../include/HttpServer.hpp"
#include "../include/Dispatcher/SimRequest.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
HttpServer::HttpServer(int port, const std::string& address):
	mIp(address),
	mPort(port)
{
	mServer.Get("/", [](const httplib::Request& req, httplib::Response& res) {
		res.set_content("Hello, World!", "text/plain");
	});

	mServer.Get("/DataRef", [&](auto& req, auto& res) {
		if (req.get_param_value("link").empty()) {
			res.status = 400;
			res.set_content("Missing 'link' parameter", "text/plain");
			return;
		}

		SimRequest request;
		request.id = mRequestId++;
		request.Type = RequestModule::DataRef;
		DataRefType type = DataRefType::Undefined;
		if (req.get_param_value("type").empty() == false) {
			type = DataRefTypeFromString(req.get_param_value("type"));
		}

		request.Payload = DataRefRequest
		{
			DataRefRequest::Get,
			req.get_param_value("link"),
			type,
			std::nullopt
		};

#ifdef _DEBUG
		res.set_content(request.Serialize(), "application/json");
#else
		res.set_content("DataRef GET", "text/plain");
#endif
	});

	mServer.Post("/DataRef", [&](auto& req, auto& res) {
		res.set_content("DataRef SET", "text/plain");
	});

	mServer.Get("/Command", [&](auto& req, auto& res) {
		res.set_content("Command", "text/plain");
	});
}

void HttpServer::start()
{
	mServer.listen(mIp.c_str(), mPort);
}
