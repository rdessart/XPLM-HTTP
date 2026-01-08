#pragma once
#include <unordered_map>
#include <mutex>

#include "Dispatcher/SimRequest.hpp"

class RequestRegistry
{
public:
	std::future<SimResponse> create(uint64_t id);
	void fulfill(uint64_t id, SimResponse&& response);

private:
	std::unordered_map<uint64_t, RequestContext> mContexts;
	std::mutex mMutex;
};

