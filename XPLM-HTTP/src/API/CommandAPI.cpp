#include "CommandAPI.hpp"

#include <API/HttpEndPoint.hpp>

CommandAPI::CommandAPI():
	BaseAPI("Command")
{
}

std::vector<HttpEndPoint> CommandAPI::getEndPoints()
{
	 std::vector<HttpEndPoint> ep;

	 addEndpoint(ep, "/once" , HttpMethods::GET, static_cast<int>(CommandExecution::Once));
	 addEndpoint(ep, "/begin", HttpMethods::GET, static_cast<int>(CommandExecution::Begin));
	 addEndpoint(ep, "/end"  , HttpMethods::GET, static_cast<int>(CommandExecution::End));
     addEndpoint(ep, "/list", HttpMethods::GET,  static_cast<int>(CommandExecution::List));
	 return ep;
}

HttpResponse CommandAPI::Handle(int requestId, httplib::Request const req)
{
	uint64_t actionId = generateId();
	spdlog::info("New Operation with id : {}", actionId);
    CommandExecution command = static_cast<CommandExecution>(requestId);
    if (command < CommandExecution::List && req.get_param_value("link").empty())
    {
        spdlog::critical("[{}] : MISSING LINK PARAMETERS", actionId);
        return HttpResponse
        {
            .isSuccess = false,
            .statusMessage = "error:missing parameter 'link'",
            .returnMessage{
                {"Message", "Missing 'link' mandatory parameter"}
            }
        };
    }
    if (command == CommandExecution::List)
    {
        json result;
        for (auto& kv : mCommandCache)
        {
            result.push_back(kv.first);
        }
        return HttpResponse::Ok(result);
    }

    CommandOperation ops{
        .link = req.get_param_value("link"),
        .executionMode = command
    };

    return Execute(actionId, ops);
}

void CommandAPI::MainThreadHandle()
{
    uint64_t id;
    CommandOperation data;
    HttpResponse response = HttpResponse::UnexpectedError();

    if (!GetDataMainThread(id, data)) {
        return;
    }

    XPLMCommandRef command = getCommand(data.link);
    if (command == nullptr)
    {
        spdlog::critical("[{}] command with link: '{}' is nullptr (invalid)", id, data.link);
        response.statusMessage = "error:command not found";
        SetReturnMainThread(id, response);
        return;
    }

    switch (data.executionMode)
    {
    case CommandExecution::Once:
        XPLMCommandOnce(command);
        response.statusMessage = "ok:once";
        break;
    case CommandExecution::Begin:
        XPLMCommandBegin(command);
        response.statusMessage = "ok:begin";
        break;
    case CommandExecution::End:
        XPLMCommandEnd(command);
        response.statusMessage = "ok:end";
        break;
    case CommandExecution::List:
        // List is not an execution mode
        response.statusMessage = "error:invalid execution mode";
        response.isSuccess = false;
        break;
    }
    response.isSuccess = true;
    SetReturnMainThread(id, response);
}

XPLMCommandRef CommandAPI::getCommand(const std::string& link)
{
    if (mCommandCache.contains(link))
        return mCommandCache.at(link);
    XPLMCommandRef command = XPLMFindCommand(link.c_str());
    if (command != nullptr)
    {
        mCommandCache.emplace(link, command);  
    }
    return command;
}
