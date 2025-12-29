#include <HttpServer.hpp>
#include <XPLM/XPLMDefs.h>
#include <XPLM/XPLMProcessing.h>

#include <Dispatcher/SimDispatcher.hpp>
#include "Datarefs/DataRefManager.hpp"
#include "Commands/CommandManager.hpp"


static ThreadSafeQueue<SimRequest>  requestQueue;
static ThreadSafeQueue<SimResponse> responseQueue;
static DataRefManager datarefManager;
static CommandManager commandManager;
static RequestRegistry registry;

static SimDispatcher dispatcher(datarefManager, commandManager, registry, requestQueue);
static HttpServer httpServer(registry, requestQueue);
static XPLMFlightLoopID serverCallbackID = nullptr;

float handleRequestCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon)
{
	dispatcher.process();
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
	httpServer.init(8080, "127.0.0.1");
	XPLMCreateFlightLoop_t serverCallback
	{
		sizeof(XPLMCreateFlightLoop_t),
		xplm_FlightLoop_Phase_AfterFlightModel,
		handleRequestCallback,
		nullptr
	};
	serverCallbackID = XPLMCreateFlightLoop(&serverCallback);
	XPLMScheduleFlightLoop(serverCallbackID, -1.0, 0);
	httpServer.start();
	return 1;
}

PLUGIN_API void XPluginDisable() 
{
	XPLMDestroyFlightLoop(serverCallbackID);
	httpServer.stop();
}

PLUGIN_API void XPluginStop() 
{

}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, int inMessage, void* inParam) 
{

}