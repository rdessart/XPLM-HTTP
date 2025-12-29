#pragma once

#include <string>
#include <variant>
#include <optional>
#include <future>
#include "../Simulator/DataRefType.hpp"
#include "../Simulator/DataRefValue.hpp"
#include "../Simulator/ICommandExecutor.hpp"
#include "../Simulator/CommandMode.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


// Module request enum, if module are added we need to add them here
enum class RequestModule {
	DataRef,
	Command,
};

//Implementation of module
struct DataRefRequest {
	enum DataRefOperation {
		Get,
		Set,
	};

	DataRefOperation Operation;
	std::string Link;
	DataRefType Type = DataRefType::Undefined;
	std::optional<json> Value = std::nullopt;

};
struct CommandRequest {

	CommandMode Operation;
	std::string Link;
};

//Payload of the request will contain the implemented request module
using SimRequestPayload = std::variant<
	DataRefRequest,
	CommandRequest>;


//Complete request structure
struct SimRequest {
	uint64_t id;
	RequestModule Type;
	SimRequestPayload Payload;

#ifdef _DEBUG
	std::string Serialize() {
		json j;
		j["request_id"] = id;
		j["module"] = static_cast<int>(Type);
		// Serialize payload based on module type
		switch (Type) {
			case RequestModule::DataRef: {
				const auto& dataRefReq = std::get<DataRefRequest>(Payload);
				j["payload"]["operation"] = static_cast<int>(dataRefReq.Operation);
				j["payload"]["link"] = dataRefReq.Link;
				j["payload"]["type"] = static_cast<int>(dataRefReq.Type);
				if (dataRefReq.Value.has_value()) {
					// Note: Actual serialization of DataRefValue would depend on its type
					j["payload"]["value"] = "ValuePresent"; // Placeholder
				} else {
					j["payload"]["value"] = nullptr;
				}
				break;
			}
			case RequestModule::Command: {
				const auto& cmdReq = std::get<CommandRequest>(Payload);
				j["payload"]["operation"] = static_cast<int>(cmdReq.Operation);
				j["payload"]["link"] = cmdReq.Link;
				break;
			}
		}
		return j.dump();
	}
#endif // _DEBUG

};

struct SimResponse {
	uint64_t id;
	bool success;
	json value;
	std::string error;
};

struct RequestContext {
	uint64_t id;
	std::promise<SimResponse> promise;
};
