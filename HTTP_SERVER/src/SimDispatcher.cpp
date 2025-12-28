#include "../include/Dispatcher/SimDispatcher.hpp"

#include <spdlog/sinks/rotating_file_sink.h>

//extern ThreadSafeQueue<SimRequest>  gRequestQueue;
//extern ThreadSafeQueue<SimResponse> gResponseQueue;

SimDispatcher::SimDispatcher(IDataProvider& drm, ICommandExecutor& cm, 
	ThreadSafeQueue<SimRequest>& request, ThreadSafeQueue<SimResponse>& response):
	dataRefs(drm), commands(cm), mRequestQueue(request), mResponseQueue(response)
{
	auto max_size = 1048576 * 5;
	auto max_files = 3;
	mLogger = spdlog::rotating_logger_mt("simDispatcher", "logs/simDispatcher.txt", max_size, max_files);
}

void SimDispatcher::process()
{
	SimRequest request;
	while (mRequestQueue.try_pop(request)) {
		spdlog::info("Processing request id={} module={}", request.id, static_cast<int>(request.Type));
		SimResponse response;
		response.id = request.id;
		response.success = false;
		switch (request.Type) {
			case RequestModule::DataRef: {
				const auto& dataRefReq = std::get<DataRefRequest>(request.Payload);
				switch (dataRefReq.Operation)
				{
				case DataRefRequest::Get:
				{
					auto result = dataRefs.get(dataRefReq.Link);
					if (result.has_value()) {
						response.success = true;
						response.value = DataRefValueToJson(result.value()); // Placeholder
					}
					else {
						response.error = "DataRef not found";
					}
					break;
				}
				case DataRefRequest::Set:
				{
					if (dataRefReq.Value.has_value()) {
						bool setResult = dataRefs.set(dataRefReq.Link, *(dataRefReq.Value));
						if (setResult) {
							response.success = true;
						}
						else {
							response.error = "Failed to set DataRef";
						}
					}
					else {
						response.error = "No value provided for DataRef set operation";
					}
					break;
				}
				default:
					break;
				}
			break;
			}
			case RequestModule::Command: 
			{
				const auto& cmdReq = std::get<CommandRequest>(request.Payload);
				try {
					commands.execute(cmdReq.Link, cmdReq.Operation);
					response.success = true;
				} catch (const std::exception& e) {
					response.error = e.what();
				}
				break;
			}
			default:
				response.error = "Unknown request module";
				break;
		}
		mResponseQueue.push(response);
	}
}
