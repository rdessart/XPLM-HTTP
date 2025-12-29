#pragma once

#include "DataRefValue.hpp"

struct SimResponse;
struct SimRequest;

class IDataRefManager
{
public:
    virtual SimResponse handle(const SimRequest&) = 0;
    virtual ~IDataRefManager() = default;
};