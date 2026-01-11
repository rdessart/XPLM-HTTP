#include "PositionAPI.h"

PositionAPI::PositionAPI() : BaseAPI("Position")
{
	addDataref("latitude", "sim/flightmodel/position/latitude", DataRefType::Double);
	addDataref("longitude", "sim/flightmodel/position/longitude", DataRefType::Double);
	addDataref("elevation", "sim/flightmodel/position/elevation", DataRefType::Double);
	addDataref("pitch", "sim/flightmodel/position/theta", DataRefType::Float);
	addDataref("roll", "sim/flightmodel/position/phi", DataRefType::Float);
	addDataref("heading", "sim/flightmodel/position/psi", DataRefType::Float);
	addDataref("ground_speed", "sim/flightmodel2/position/groundspeed", DataRefType::Undefined);
	addDataref("true_airspeed", "sim/flightmodel2/position/true_airspeed", DataRefType::Undefined);
	addDataref("equiv_airspeed", "sim/flightmodel/position/equivalent_airspeed", DataRefType::Undefined);
	addDataref("cas", "sim/cockpit2/gauges/indicators/calibrated_airspeed_kts_pilot", DataRefType::Undefined);
	addDataref("mach", "sim/cockpit2/gauges/indicators/mach_pilot", DataRefType::Undefined);
	addDataref("alpha", "sim/flightmodel2/position/alpha", DataRefType::Undefined);
	addDataref("beta", "sim/flightmodel2/position/beta", DataRefType::Undefined);
}

std::vector<HttpEndPoint> PositionAPI::getEndPoints()
{
	std::vector<HttpEndPoint> endpoints;
	addEndpoint(endpoints, "", HttpMethods::GET, 1);
	addEndpoint(endpoints, "/list", HttpMethods::GET, 2);

	return endpoints;
}

HttpResponse PositionAPI::Handle(int requestId, httplib::Request const req)
{
	if (requestId == 1)
	{
		json result = json::object();
		for (auto const& [key, value] : mDataRefsValue)
		{
			result[key] = value;
		}
		return HttpResponse::Ok(result);
	}
	if(requestId == 2)
	{
		json result = json::array();
		for (auto const& [key, _] : mDataRefs)
		{
			result.push_back(key);
		}
		return HttpResponse::Ok(result);
	}
}

void PositionAPI::MainThreadHandle()
{
	//find any dataref that is not yet loaded
	for (auto& [name, pathType] : mDataRefPromise)
	{
		DataRef dr = DataRef(pathType.first, pathType.second);
		if (!dr.isValid())
		{
			mLogger->error("PositionAPI: failed to load dataref {} with path {} (type = {})", name, pathType.first, static_cast<int>(pathType.second));
			continue;
		}
		mDataRefs.emplace(name, dr);
		mDataRefsValue.emplace(name, json());
	}
	mDataRefPromise.clear();

	//update all dataref values
	for (auto& [name, dataRefOpt] : mDataRefs)
	{
		if (!dataRefOpt.has_value())
			continue;
		json value;
		if (!dataRefOpt->getValue(value))
		{
			mLogger->error("PositionAPI: failed to get value for dataref {}", name);
			continue;
		}
		mDataRefsValue[name] = value;
	}
}

void PositionAPI::addDataref(const std::string& name, const std::string& path, DataRefType type)
{
	mDataRefPromise.emplace(name, std::make_pair(path, type));
}