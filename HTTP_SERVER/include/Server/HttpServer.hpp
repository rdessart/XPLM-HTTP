#pragma once
#include <httplib.h>
#include <spdlog/spdlog.h>

#include <string>
#include <thread>
#include <future>
#include <unordered_map>

class IBaseAPI;

class HttpServer
{
public:
	HttpServer();
	void SetListeningAddress(const std::string& ip, int port);
	void RegisterApi(IBaseAPI& api);
	void Start();
	std::string getBaseAddress() const;

	void cors_data(std::unordered_map<std::string, std::string> const& headers);

protected:
	httplib::Server mServer;
	std::string mIp = "localhost";
	int mPort = 80;
	std::thread mThread;
	bool mRunning;
	std::map<std::string, IBaseAPI&> mApiMap;
	std::shared_ptr<spdlog::logger> mLogger;

	std::unordered_map<std::string, std::string> mConfigHeaders;

private:
	void Init();
	void Run();
};

