#include <spdlog/sinks/rotating_file_sink.h>

#include "../include/Dispatcher/SimDispatcher.hpp"


SimDispatcher::SimDispatcher(IDataRefManager& drm, ICommandExecutor& cm, RequestRegistry& registery, ThreadSafeQueue<SimRequest>& requestQueue) :
	mDataRefManager(drm), mCommandExecutor(cm), mRequestRegistery(registery), mRequestQueue(requestQueue)
{
	auto max_size = 1048576 * 5;
	auto max_files = 3;
	mLogger = spdlog::rotating_logger_mt("simDispatcher", 
		"logs/simDispatcher.txt", max_size, max_files);
}

void SimDispatcher::process()
{
	SimRequest request;
	while (mRequestQueue.try_pop(request)) {
		spdlog::info("Processing request id={} module={}", request.id, static_cast<int>(request.Type));
		SimResponse response{};
		switch (request.Type) {
			case RequestModule::DataRef:
				response = mDataRefManager.handle(request);
				break;
			case RequestModule::Command: 
			{
				response = mCommandExecutor.handle(request);
				//const auto& cmdReq = std::get<CommandRequest>(request.Payload);
				//try {
				//	mCommandExecutor.execute(cmdReq.Link, cmdReq.Operation);
				//	response.success = true;
				//} catch (const std::exception& e) {
				//	response.error = e.what();
				//}
				break;
			}
			default:
				response.error = "Unknown request module";
				break;
		}
		//mResponseQueue.push(response);
		mRequestRegistery.fulfill(response.id, std::move(response));
	}
}
