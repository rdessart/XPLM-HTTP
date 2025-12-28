#pragma once
#include <memory>
#include <thread>

#include <httplib.h>
#include <spdlog/spdlog.h>

#include "../include/Dispatcher/SimRequest.hpp"
#include "../include/Dispatcher/SimResponse.hpp"
#include "../include/ThreadSafeQueue.hpp"
class HttpServer
{
public:
	HttpServer(ThreadSafeQueue<SimRequest>& requestQueue, ThreadSafeQueue<SimResponse>& responseQueue);
	void init(int port = 8080, const std::string& address = "127.0.0.1");
	void start();
	void stop();
	int getPort() const { return mPort; }
	std::string getIpAddress() const { return mIp; }

private:
	void run();
	void sendResponse(uint64_t id, httplib::Response& res);
	httplib::Server mServer;
	std::string mIp;
	int mPort;
	bool mRunning;
	uint64_t mRequestId = 0;
	std::shared_ptr<spdlog::logger> mLogger;
	ThreadSafeQueue<SimRequest>& mRequestQueue;
	ThreadSafeQueue<SimResponse>& mResponseQueue;
	std::thread mThread;
	
};

