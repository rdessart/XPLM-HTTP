#include <iostream>
#include <HttpServer.hpp>
#include <Dispatcher/SimDispatcher.hpp>

#include "DummyDatarefManager.hpp"
#include "DummyCommandManager.hpp"

int main(int argc, char* argv[])
{
	ThreadSafeQueue<SimRequest>  requestQueue;
	ThreadSafeQueue<SimResponse> responseQueue;
	
	DummyDatarefManager datarefManager;
	DummyCommandManager commandManager;

	SimDispatcher dispatcher(datarefManager, commandManager, requestQueue, responseQueue);
	HttpServer server(requestQueue, responseQueue);

	server.init(8080, "127.0.0.1");
	std::cout << "Starting HTTP Server at " << server.getIpAddress()
		<< ":" << server.getPort() << "\n";

	server.start();
	
	while (true) {
		dispatcher.process();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}


	server.stop();
	return 0;
}