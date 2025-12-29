#pragma once
#include <unordered_map>

#include <XPLM/XPLMUtilities.h>

#include <Dispatcher/SimRequest.hpp>
#include <Simulator/ICommandExecutor.hpp>


class CommandManager: public ICommandExecutor
{
public:
    //void execute(const std::string& command, CommandMode mode) override;
    SimResponse handle(const SimRequest& request) override;
private:
    XPLMCommandRef resolve(const std::string& name);
    std::unordered_map<std::string, XPLMCommandRef> mCache;
};