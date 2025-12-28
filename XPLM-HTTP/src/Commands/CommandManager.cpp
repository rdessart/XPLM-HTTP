#include "CommandManager.hpp"

void CommandManager::execute(const std::string& command, CommandMode mode)
{
	XPLMCommandRef cmd = resolve(command);
	if (cmd == nullptr) return;
	switch (mode)
	{
	case CommandMode::Once:
		XPLMCommandOnce(cmd);
		break;
	case CommandMode::Begin:
		XPLMCommandBegin(cmd);
		break;
	case CommandMode::End:
		XPLMCommandEnd(cmd);
		break;
	}
}

XPLMCommandRef CommandManager::resolve(const std::string& name)
{
    if(mCache.contains(name))
		return mCache[name];
	XPLMCommandRef cmd = XPLMFindCommand(name.c_str());
	if (cmd != nullptr) mCache[name] = cmd;
	return cmd;
}
