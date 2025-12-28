#include <iostream>

#include <HttpServer.hpp>


int main(int argc, char* argv[])
{
	HttpServer server(8080, "127.0.0.1");
	std::cout << "Starting HTTP Server at " << server.getIpAddress()
		<< ":" << server.getPort() << "\n";
	server.start();
	return 0;
}