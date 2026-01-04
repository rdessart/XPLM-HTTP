#include "CommandAPI.hpp"

#include <API/HttpEndPoint.hpp>

CommandAPI::CommandAPI():
	BaseAPI("Command")
{
}

std::vector<HttpEndPoint> CommandAPI::getEndPoints()
{
	 std::vector<HttpEndPoint> ep;

	 addEndpoint(ep, "/once" , HttpMethods::GET, (int)CommandExecution::Once);
	 addEndpoint(ep, "/begin", HttpMethods::GET, (int)CommandExecution::Begin);
	 addEndpoint(ep, "/end"  , HttpMethods::GET, (int)CommandExecution::End);

	 return ep;
}

HttpResponse CommandAPI::Handle(int requestId, httplib::Request const req)
{
	uint64_t actionId = generateId();
	spdlog::info("New Operation with id : {}", actionId);

    if (req.get_param_value("link").empty()) {
        spdlog::critical("[{}] : MISSING LINK PARAMETERS", actionId);
        return HttpResponse
        {
            .isSuccess = false,
            .statusMessage = "error:missing link",
            .returnMessage{
                {"Message", "Missing 'link' mandatory parameter"}
            }
        };
    }

    CommandOperation ops{
        .link = req.get_param_value("link"),
        .executionMode = (CommandExecution)requestId
    };

    std::future<HttpResponse> resp = send_request(actionId, ops);
    if (resp.wait_for(DefaultTimeout) == std::future_status::ready)
    {
        return resp.get();
    }
    spdlog::critical("[{}] : HAS TIMED OUT", actionId);

    return HttpResponse{
        .isSuccess = false,
        .statusMessage = "timeout",
        .returnMessage{
            {"Message", "Request has timed out (>500ms)"}
        }
    };
}

void CommandAPI::MainThreadHandle()
{
    uint64_t id;
    CommandOperation data;
    HttpResponse response{
        .isSuccess = false,
        .statusMessage = "error:unexpectedError"
    };

    bool res = GetDataMainThread(id, data);
    if (!res) {
        spdlog::critical("An unexpected error has occured while getting data for thread execution");
        SetReturnMainThread(id, response);
        return;
    }
    XPLMCommandRef command = nullptr;
    if (!mCommandCache.contains(data.link))
    {
        command = XPLMFindCommand(data.link.c_str());
        if (command != nullptr)
            mCommandCache.emplace(data.link, command);
    }
    else {
        command = mCommandCache[data.link];
    }

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
    }
    response.isSuccess = true;
    SetReturnMainThread(id, response);
}
