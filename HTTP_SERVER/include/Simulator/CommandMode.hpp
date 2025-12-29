#pragma once
#include <optional>
#include <string>

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