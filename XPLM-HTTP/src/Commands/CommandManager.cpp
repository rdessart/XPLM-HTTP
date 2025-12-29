#include "CommandManager.hpp"

SimResponse CommandManager::handle(const SimRequest& request)
{
	SimResponse response{
		request.id,
	};
	const auto& cmdReq = std::get<CommandRequest>(request.Payload);
	XPLMCommandRef command = resolve(cmdReq.Link);
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
			response.value = "executed once";
			break;
		case CommandMode::Begin:
			XPLMCommandBegin(command);
			response.success = true;
			response.value = "begin coninious execution";
			break;
		case CommandMode::End:
			XPLMCommandEnd(command);
			response.success = true;
			response.value = "ended coninious execution";
			break;
	}

	return response;
}

XPLMCommandRef CommandManager::resolve(const std::string& name)
{
    if(mCache.contains(name))
		return mCache[name];
	XPLMCommandRef cmd = XPLMFindCommand(name.c_str());
	if (cmd != nullptr) mCache[name] = cmd;
	return cmd;
}
