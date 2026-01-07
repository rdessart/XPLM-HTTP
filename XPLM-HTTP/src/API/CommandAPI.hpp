#pragma once
#include <API/BaseAPI.hpp>

#include <unordered_map>
#include <XPLM/XPLMUtilities.h>


enum class CommandExecution {
	Once,
	Begin,
	End,
	List
};

struct CommandOperation {
	std::string link;
	CommandExecution executionMode;
};

class CommandAPI:  
	public BaseAPI<CommandOperation, HttpResponse>
{
public:
	CommandAPI();
	std::vector<HttpEndPoint> getEndPoints() override;
	HttpResponse Handle(int endpointIdentificator, httplib::Request const req) override;
	void MainThreadHandle() override;

private:
	std::unordered_map<std::string, XPLMCommandRef> mCommandCache;
	XPLMCommandRef getCommand(const std::string& link);
};

