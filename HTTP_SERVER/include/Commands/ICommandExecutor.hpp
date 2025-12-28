#pragma once
#include <string>
#include <optional>

enum class CommandMode {
    Once,
    Begin,
    End
};

static inline std::optional<CommandMode> StringToCommand(const std::string& input)
{
    if (input == "once") return CommandMode::Once;
    if (input == "begin") return CommandMode::Begin;
    if (input == "end") return CommandMode::End;
    return std::nullopt;
}

class ICommandExecutor {
public:
    virtual void execute(const std::string& command, CommandMode mode) = 0;
    virtual ~ICommandExecutor() = default;
};