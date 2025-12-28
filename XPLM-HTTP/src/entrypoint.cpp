#include <XPLM/XPLMDefs.h>

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) 
{
	strcpy_s(outName, 256, "XPLM-HTTP");
	strcpy_s(outSig, 256, "eSkyStudio.Network.http");
	strcpy_s(outDesc, 256, "A X-Plane webserver for dataref access, command exectution and websocket");
	return 1;
}

PLUGIN_API void XPluginEnable() 
{

}

PLUGIN_API void XPluginDisable() 
{

}

PLUGIN_API void XPluginStop() 
{

}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, int inMessage, void* inParam) 
{

}