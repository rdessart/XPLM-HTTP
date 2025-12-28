#pragma once
#include <string>

enum class CommandMode {
    Once,
    Begin,
    End
};

class ICommandExecutor {
public:
    virtual void execute(const std::string& command, CommandMode mode) = 0;
    virtual ~ICommandExecutor() = default;
};