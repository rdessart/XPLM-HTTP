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
};