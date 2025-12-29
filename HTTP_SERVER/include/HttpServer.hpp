#pragma once
#include <memory>
#include <thread>
#include <future>

#include <httplib.h>
#include <spdlog/spdlog.h>

#include "RequestRegistry.hpp"
#include "Dispatcher/SimRequest.hpp"
#include "ThreadSafeQueue.hpp"

class HttpServer
{
public:
	HttpServer(RequestRegistry& registery,
		ThreadSafeQueue<SimRequest>& requestQueue);
	void init(int port = 8080, const std::string& address = "127.0.0.1");
	void start();
	void stop();
	int getPort() const { return mPort; }
	std::string getIpAddress() const { return mIp; }

private:
	void run();
	//void sendResponse(uint64_t id, httplib::Response& res);
	httplib::Server mServer;
	std::string mIp;
	int mPort;
	bool mRunning;
	std::atomic<uint64_t> mRequestId;
	std::shared_ptr<spdlog::logger> mLogger;
	ThreadSafeQueue<SimRequest>& mRequestQueue;
	RequestRegistry& mRegistry;
	std::thread mThread;
	static constexpr std::chrono::milliseconds DefaultTimeout{ 500 };

private:
	void sendResponse(SimRequest const request, httplib::Response& res);

	uint64_t generateId()
	{
		return mRequestId.fetch_add(1, std::memory_order_relaxed) + 1;
	}


};
