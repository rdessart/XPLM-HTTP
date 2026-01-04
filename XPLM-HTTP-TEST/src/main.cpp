#include <iostream>
#include <Server/HttpServer.hpp>
#include "API/DummyDataRefAPI.hpp"

int main(int argc, char* argv[])
{
	HttpServer server;
	DummyDataRefAPI dataRefApi;

	server.RegisterApi(dataRefApi);
	server.Start();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		dataRefApi.MainThreadHandle();
	}
	return 0;
}