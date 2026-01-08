#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct HttpResponse {
	bool isSuccess;
	std::string statusMessage;
	json returnMessage;

	json Serialize() {
		return json{
			{"Sucess", isSuccess},
			{"Status", statusMessage},
			{"Value", returnMessage}
		};
	}

	static HttpResponse Ok() {
		return HttpResponse{
			.isSuccess = true,
			.statusMessage = "ok"
		};
	}

	static HttpResponse Ok(json value) {
		return HttpResponse{
			.isSuccess = true,
			.statusMessage = "ok",
			.returnMessage = value
		};
	}

	static HttpResponse NotFound() 
	{
		return HttpResponse{
			.isSuccess = false,
			.statusMessage = "error:NotFound"
		};
	}

	static HttpResponse TimedOut()
	{
		return HttpResponse{
			.isSuccess = false,
			.statusMessage = "timeout",
			.returnMessage{
				{"Message", "Request has timed out (>500ms)"}
			}
		};
	}

	static HttpResponse UnexpectedError()
	{
		return HttpResponse{
			.isSuccess = false,
			.statusMessage = "error:unexpectedError"
		};
	}
};