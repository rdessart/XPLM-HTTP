#pragma once
#include <spdlog/spdlog.h>
#include "SimRequest.hpp"
#include "SimResponse.hpp"

#include "../ThreadSafeQueue.hpp"
#include "../DataRefs/IDataProvider.hpp"
#include "../Commands/ICommandExecutor.hpp"

class SimDispatcher {
public:
    SimDispatcher(IDataProvider& drm, ICommandExecutor& cm, 
        ThreadSafeQueue<SimRequest>& request, ThreadSafeQueue<SimResponse>&response);
    void process();

private:
    IDataProvider& dataRefs;
    ICommandExecutor& commands;
    std::shared_ptr<spdlog::logger> mLogger;
    ThreadSafeQueue<SimRequest>& mRequestQueue;
    ThreadSafeQueue<SimResponse>& mResponseQueue;
};