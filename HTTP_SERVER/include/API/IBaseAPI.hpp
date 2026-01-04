#pragma once

#include<string>
#include<vector>

#include "HttpEndPoint.hpp"

/// <summary>
/// API Interface. To be used by an HTTP Server
/// </summary>
class IBaseAPI {
public:
    virtual ~IBaseAPI() = default;
    virtual std::string getName() const = 0;
    virtual std::vector<HttpEndPoint> getEndPoints() = 0;
    virtual void MainThreadHandle() = 0;
};