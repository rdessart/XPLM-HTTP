#pragma once

#include <httplib.h>

class HttpServer
{
	public:
	HttpServer(int port = 8080, const std::string& address = "127.0.0.1");
	void start();

	int getPort() const { return mPort; }
	std::string getIpAddress() const { return mIp; }

	private:
		httplib::Server mServer;
		std::string mIp;
		int mPort;
		uint64_t mRequestId = 0;
};

