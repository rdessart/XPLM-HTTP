#pragma once
#include <string>
#include <optional>
#include "DataRefValue.hpp"
#include "CommandMode.hpp"

struct SimRequest;
struct SimResponse;

class ICommandExecutor {
public:
    virtual SimResponse handle(const SimRequest&) = 0;
    virtual ~ICommandExecutor() = default;
};