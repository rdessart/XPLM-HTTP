#pragma once
#include <queue>
#include <condition_variable>
#include <future>
#include <map>
#include <mutex>
#include <httplib.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "HttpEndPoint.hpp"
#include "IBaseAPI.hpp"

template<typename TReq, typename TRes>
class BaseAPI : public IBaseAPI
{
private:
	
public:
	BaseAPI(const std::string& name) : 
		mName(name)
	{
		auto max_size = 1048576 * 5;
		auto max_files = 3;
		std::stringstream path;
		path << "log/" << mName << ".log";
		mLogger = spdlog::rotating_logger_mt(mName, path.str(), max_size, max_files);
		spdlog::set_default_logger(mLogger);
		spdlog::flush_on(spdlog::level::trace);
	}

	virtual ~BaseAPI() = default;


	/// <summary>
	/// Returns the collection of HTTP endpoints. This member function is virtual and can be overridden by derived classes.
	/// </summary>
	/// <returns>A std::vector<HttpEndPoint> containing the endpoint objects.</returns>
	virtual std::vector<HttpEndPoint> getEndPoints() override = 0;

	/// <summary>
	/// Returns the object's name.
	/// </summary>
	/// <returns>A copy of the stored name as a std::string. The function is const and does not modify the object.</returns>
	std::string getName() const override { return mName; };

	///	<summary> function to prepare the request to be called on the main thread
	/// <para>
	///	this function is called when request hit the server. Her role is to prepare request to be executed
	///	by the sim thread at next execution and checking parameters and arguments.
	/// </para>
	/// <para>
	///	When ready the function will put the prepared request on the synchronisation mechanisme and wait
	///	for the result.
	/// </para>
	/// </summary>
	virtual TRes Handle(int requestId, httplib::Request const req) = 0;

	///<summary>callback function to be call on the main thread
	///<para>Function with promise to be call on X - Plane(or main) thread with full API ACCESS</para>
	///</summary>
	virtual void MainThreadHandle() override = 0;


protected:
	std::shared_ptr<spdlog::logger> mLogger;
	std::future<TRes> send_request(uint64_t id, TReq request) {
		std::lock_guard<std::mutex> lock(mMutex);
		auto [it, inserted] = mResponseQueue.emplace(id, std::promise<TRes> {});
		std::future<TRes> future = it->second.get_future();

		mRequestStore[id] = std::move(request);
		mWorkQueue.push(id);
		mCv.notify_one();
		return future;
	}

	uint64_t generateId()
	{
		mId++;
		return mId;
	};

	void addEndpoint(std::vector<HttpEndPoint>& list,
		const std::string& path,
		HttpMethods method, 
		int endpointIdentificator = 0)
	{
		list.push_back({
			path,
			method,
			[this](int id, httplib::Request req) { return this->Handle(id, req); },
			endpointIdentificator
			});
	}

	bool SetReturnMainThread(uint64_t id, TRes const data)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		if (!mResponseQueue.contains(id))
		{
			return false;
		}
		mResponseQueue[id].set_value(std::move(data));
		mResponseQueue.erase(id);
		return true;
	}

	bool GetDataMainThread(uint64_t& outId, TReq& outData)
	{
		std::unique_lock<std::mutex> lock(mMutex);
		if (mWorkQueue.empty()) return false;
		mCv.wait(lock, [this] {return !mWorkQueue.empty(); });
		outId = mWorkQueue.front();
		mWorkQueue.pop();
		outData = std::move(mRequestStore[outId]);
		mRequestStore.erase(outId);

		return true;
	}

protected:
	uint64_t mId = 0;
	std::mutex mMutex;
	std::condition_variable mCv;

	/// <summary>
	/// The name of the API
	/// </summary>
	std::string mName;

	/// <summary>
	/// List of ID to be treated
	/// </summary>
	std::queue<uint64_t> mWorkQueue;

	/// <summary>
	/// The queue of output data as promise. 
	/// TO BE IMPLEMENTED BY CHILD CLASSES
	/// </summary>
	std::unordered_map<uint64_t, std::promise<TRes>> mResponseQueue;
	
	/// <summary>
	/// The queue of input data. 
	/// TO BE IMPLEMENTED BY CHILD CLASSES
	/// </summary>
	std::unordered_map<uint64_t, TReq> mRequestStore;

	/// <summary>
	/// the default timeout to reject the request!
	/// </summary>
	static constexpr std::chrono::milliseconds DefaultTimeout{ 500 };
};

