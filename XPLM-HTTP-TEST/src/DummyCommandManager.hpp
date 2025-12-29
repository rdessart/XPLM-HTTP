#pragma once

#include <Simulator/ICommandExecutor.hpp>
#include <iostream>

class DummyCommandManager : public ICommandExecutor
{
	public:
	DummyCommandManager() = default;
	SimResponse handle(const SimRequest& request) override {
		SimResponse response{
		request.id,
		};
		const auto& cmdReq = std::get<CommandRequest>(request.Payload);
		std::cout << "Executing command: " << cmdReq.Link << "with mode : " << static_cast<int>(cmdReq.Operation) << "\n";
		/*XPLMCommandRef command = resolve(cmdReq.Link);
		if (command == nullptr)
		{
			response.success = false;
			response.error = "Command hasn't been found!";
			return response;
		}

		switch (cmdReq.Operation)
		{
		case CommandMode::Once:
			XPLMCommandOnce(command);
			response.success = true;
			response.value = "":
		default:
			break;
		}*/

		return response;
	}
};

