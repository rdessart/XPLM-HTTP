#include <Server/HttpServer.hpp>
#include <API/IBaseAPI.hpp>

#include "configuration.hpp"
#include "API/CommandAPI.hpp"
#include "API/DataRefAPI.hpp"
#include "API/PositionAPI.hpp"


#include <XPLM/XPLMDefs.h>
#include <XPLM/XPLMProcessing.h>
#include <vector>

static std::vector<IBaseAPI*> apis = {};
static HttpServer server;
static XPLMFlightLoopID serverCallbackID = nullptr;

float handleRequestCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	for (auto api : apis)
	{
		api->MainThreadHandle();
	}
	return -1.0;
}

float initCallBack(float inElapsedLC, float inElapsedLFL, int inCounter, void* inRefCon)
{
	XPLMRegisterFlightLoopCallback(handleRequestCallback, -1.0f, nullptr);
	for (auto api : apis)
	{
		server.RegisterApi(*api);
	}
	server.Start();

	return 0;
}

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) 
{
	Configuration config("./Resources/plugins/XPLM-HTTP/config.json");

	#ifdef IBM
	strcpy_s(outName, 256, config.pluginName.c_str());
	strcpy_s(outSig, 256, config.pluginDescription.c_str());
	strcpy_s(outDesc, 256, config.pluginSignature.c_str());
	#else
	strcpy(outName, config.pluginName.c_str());
	strcpy(outSig, config.pluginDescription.c_str());
	strcpy(outDesc, config.pluginSignature.c_str());
	#endif
	if(config.enable_crossOrigin) server.cors_data(config.headers);
	server.SetListeningAddress(config.bind_url, config.bind_port);
	
	return 1;
}

PLUGIN_API int XPluginEnable() 
{
	apis.push_back(new DataRefAPI());
	apis.push_back(new CommandAPI());
	apis.push_back(new PositionAPI());

	XPLMRegisterFlightLoopCallback(initCallBack, -1.0f, nullptr);
	return 1;
}

PLUGIN_API void XPluginDisable() 
{
	while (apis.size() > 0)
	{
		auto api = apis.back();
		delete api;
		apis.pop_back();
	}
}

PLUGIN_API void XPluginStop() 
{
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, int inMessage, void* inParam) 
{

}