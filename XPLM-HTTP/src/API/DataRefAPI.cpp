#include "DataRefAPI.hpp"

DataRefAPI::DataRefAPI() : BaseAPI("DataRef")
{
}

std::vector<HttpEndPoint> DataRefAPI::getEndPoints()
{
	std::vector<HttpEndPoint> ep;
	addEndpoint(ep, "", HttpMethods::GET, static_cast<int>(DataRefEndPoint::Get));
	addEndpoint(ep, "", HttpMethods::POST, static_cast<int>(DataRefEndPoint::Set));
	addEndpoint(ep, "/list", HttpMethods::GET, static_cast<int>(DataRefEndPoint::List));
	return ep;
}

HttpResponse DataRefAPI::Handle(int endpointIdentificator, httplib::Request const req)
{
	uint64_t actionId = generateId();
	DataRefEndPoint ops = static_cast<DataRefEndPoint>(endpointIdentificator);

	if (ops != DataRefEndPoint::List && req.get_param_value("link").empty())
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

	switch (ops)
	{
	case DataRefEndPoint::Get:
		return handleGet(req, actionId);
	case DataRefEndPoint::Set:
		return handleSet(req, actionId);
	case DataRefEndPoint::List:
		return handleList();
	default:
		break;
	}
	return HttpResponse::NotFound();
}

HttpResponse DataRefAPI::handleList()
{
	json result;
	for (auto& kv : mCache)
	{
		result.push_back(kv.first);
	}
	return HttpResponse::Ok(result);
}

HttpResponse DataRefAPI::handleGet(httplib::Request const req, uint64_t const actionId)
{
	
	DataRefOperation drOps
	{
		.execution = DataRefEndPoint::Get,
		.link = req.get_param_value("link"),
	};
	return Execute(actionId, drOps);
}

HttpResponse DataRefAPI::handleSet(httplib::Request const req, uint64_t const actionId)
{
	json body = json::parse(req.body);

	if (!body.contains("Value"))
	{
		return HttpResponse
		{
			.isSuccess = false,
			.statusMessage = "error:missing body parameter 'Value'",
			.returnMessage{
				{"Message", "Missing BODY 'Value' mandatory parameter"}
			}
		};
	}

	DataRefOperation drOps
	{
		.execution = DataRefEndPoint::Set,
		.link = req.get_param_value("link"),
		.type = DataRef::StringToType(body.value<std::string>("type", "undefined")),
		.value = body["Value"],
	};
	return Execute(actionId, drOps);
}

void DataRefAPI::MainThreadHandle()
{
	uint64_t id;
	DataRefOperation data;
	HttpResponse response = HttpResponse::UnexpectedError();

	if (!GetDataMainThread(id, data)) {
		return;
	}

	std::optional<DataRef> d = std::nullopt;
	if (mCache.contains(data.link))
		d = mCache.at(data.link);
	else
	{
		auto dataref = DataRef(data.link, data.type);
		if (dataref.isValid())
		{
			mCache.emplace(data.link, dataref);
			d = dataref;
		}
	}

	if (!d.has_value())
	{
		response.statusMessage = "error:DataRef is not valid";
		response.returnMessage = {
			{"link", data.link},
			{"error", "No valid DataRef found at location"}
		};
		SetReturnMainThread(id, response);
		return;
	}

	if (data.execution == DataRefEndPoint::Set)
	{
		if (!d.value().setValue(data.value))
		{
			response.statusMessage = "error:Unable to set dataref value";
			response.returnMessage = {
				{"link", data.link},
				{"type", d.value().getType()},
				{"isReadOnly", d.value().isReadOnly()}
			};
			SetReturnMainThread(id, response);
			return;
		}
	}

	json returnValue;
	if(!d.value().getValue(returnValue))
	{
		response.statusMessage = "error:Unable to get dataref value";
		response.returnMessage = {
			{"link", data.link},
			{"type", d.value().getType()},
		};
	}
	else {
		response = HttpResponse::Ok(returnValue);
	}
	

	SetReturnMainThread(id, response);
}
