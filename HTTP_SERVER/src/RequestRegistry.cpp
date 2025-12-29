#include "../include/RequestRegistry.hpp"

std::future<SimResponse> RequestRegistry::create(uint64_t id)
{
    std::lock_guard lock(mMutex);
    auto& context = mContexts[id];
    context.id = id;
    return context.promise.get_future();
}

void RequestRegistry::fulfill(uint64_t id, SimResponse&& response)
{
    std::lock_guard lock(mMutex);
    auto it = mContexts.find(id);
    if (it == mContexts.end()) return;

    it->second.promise.set_value(std::move(response));
    mContexts.erase(it);
}
