#pragma once

#include "../Dispatcher/SimRequest.hpp"


struct SimRequest;
struct SimResponse;

class ISimulatorInterface
{
public:
    virtual SimResponse handle(const SimRequest&) = 0;
    virtual ~ISimulatorInterface() = default;
};