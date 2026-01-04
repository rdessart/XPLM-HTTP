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

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) 
{
	strcpy_s(outName, 256, "XPLM-HTTP");
	strcpy_s(outSig, 256, "eSkyStudio.Network.http");
	strcpy_s(outDesc, 256, "A X-Plane webserver for dataref access, command exectution and websocket");
	return 1;
}

PLUGIN_API int XPluginEnable() 
{
	server.SetListeningAddress("127.0.0.1", 8080);
	XPLMCreateFlightLoop_t serverCallback
	{
		sizeof(XPLMCreateFlightLoop_t),
		xplm_FlightLoop_Phase_AfterFlightModel,
		handleRequestCallback,
		nullptr
	};
	serverCallbackID = XPLMCreateFlightLoop(&serverCallback);
	XPLMScheduleFlightLoop(serverCallbackID, -1.0, 0);
	server.RegisterApi(commandApi);
	server.Start();
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