#pragma once

#include <Simulator/ICommandExecutor.hpp>
#include <XPLM/XPLMUtilities.h>
#include <unordered_map>

class CommandManager: public ICommandExecutor
{
public:
    //void execute(const std::string& command, CommandMode mode) override;
    SimResponse handle(const SimRequest& request) override;
private:
    XPLMCommandRef resolve(const std::string& name);
    std::unordered_map<std::string, XPLMCommandRef> mCache;
};