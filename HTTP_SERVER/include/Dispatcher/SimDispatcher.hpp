#pragma once
#include <spdlog/spdlog.h>

#include "SimRequest.hpp"
#include "../RequestRegistry.hpp"
#include "../ThreadSafeQueue.hpp"
#include "../Simulator/IDataRefManager.hpp"
#include "../Simulator/ICommandExecutor.hpp"

class SimDispatcher {
public:
    SimDispatcher(IDataRefManager& drm, ICommandExecutor& cm,
        RequestRegistry& registery, ThreadSafeQueue<SimRequest>& requestQueue);
    void process();

private:
    IDataRefManager& mDataRefManager;
    ICommandExecutor& mCommandExecutor;
    RequestRegistry& mRequestRegistery;
    std::shared_ptr<spdlog::logger> mLogger;
    ThreadSafeQueue<SimRequest>& mRequestQueue;
};