#include <Server/HttpServer.hpp>

#include <API/IBaseAPI.hpp>
#include "API/CommandAPI.hpp"
#include <XPLM/XPLMDefs.h>
#include <XPLM/XPLMProcessing.h>
#include <vector>

static std::vector<IBaseAPI> apis = {};
static HttpServer server;
static XPLMFlightLoopID serverCallbackID = nullptr;
static CommandAPI commandApi;

float handleRequestCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	commandApi.MainThreadHandle();
	return -1.0;
}

float initCallBack(float inElapsedLC, float inElapsedLFL, int inCounter, void* inRefCon)
{
	server.SetListeningAddress("127.0.0.1", 28080);

	XPLMRegisterFlightLoopCallback(handleRequestCallback, -1.0f, nullptr);

	server.RegisterApi(commandApi);
	server.Start();

	return 0;
}

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) 
{
	#ifdef IBM
	strcpy_s(outName, 256, "XPLM-HTTP");
	strcpy_s(outSig, 256, "eSkyStudio.Network.http");
	strcpy_s(outDesc, 256, "A X-Plane webserver for dataref access, command exectution and websocket");
	#else
	strcpy(outName, "XPLM-HTTP");
	strcpy(outSig, "eSkyStudio.Network.http");
	strcpy(outDesc, "A X-Plane webserver for dataref access, command exectution and websocket");
	#endif
	return 1;
}

PLUGIN_API int XPluginEnable() 
{
	XPLMRegisterFlightLoopCallback(initCallBack, -1.0f, nullptr);
	return 1;
}

PLUGIN_API void XPluginDisable() 
{
	XPLMDestroyFlightLoop(serverCallbackID);
}

PLUGIN_API void XPluginStop() 
{

}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, int inMessage, void* inParam) 
{

}