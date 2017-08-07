#include "InterfaceManager.h"
#include "interfaces\InterfaceEngine.h"
#include "interfaces\InterfaceModelInfo.h"
#include "interfaces\InterfaceEntityList.h"
#include "interfaces\InterfaceClientMode.h"
#include "interfaces\InterfaceClient.h"
#include "interfaces\InterfaceGlobals.h"
#include "interfaces\InterfaceTrace.h"
#include "interfaces\InterfacePanel.h"
#include "interfaces\InterfaceSurface.h"
#include "Address.h"
#include "Sigscan.h"
#include "Defines.h"
#include "Log.h"
#include "Utils.h"
#include <sstream>
#include <Windows.h>
#include <iomanip>

template<typename fn> fn InterfaceManager::GetInterface(std::string modulename, std::string interfacename)
{
	CreateInterfaceFn CreateInterface;
	ASSERT(CreateInterface =(CreateInterfaceFn)GetProcAddress(GetModuleHandle(const_cast<char *>(modulename.c_str())), "CreateInterface"));
	
	fn pInterface = 0;
	for (int i = 100; i > 0; i--)
	{
		std::stringstream intf;
		intf << interfacename << std::setfill('0') << std::setw(3) << i;
		pInterface = (fn)(CreateInterface(intf.str().c_str(), 0));
		if (pInterface) { 
			Log::Write(DEBUG,"%s found 0x%.8X", intf.str().c_str(), pInterface); 
			break;
		}
	}

	return pInterface;
}

void InterfaceManager::GetInterfaces()
{
	// ASSERT(g_vGui = GetInterface<CModelInfo*>("vgui2.dll", "CreateInterface"));
	g_Panel = GetInterface<IPanel*>("vgui2.dll", "VGUI_Panel");
	g_Surface = GetInterface<ISurface*>("vguimatsurface.dll", "VGUI_Surface");
	g_pClient = GetInterface<CClient*>("client.dll", "VClient");
	g_pEntityList = GetInterface<CEntityList*>("client.dll", "VClientEntityList");
	g_pEngine = GetInterface<CEngine*>("engine.dll", "VEngineClient");
	g_pModelInfo = GetInterface<CModelInfo*>("engine.dll", "VModelInfoClient");
	g_pTrace = GetInterface<CTrace*>("engine.dll", "EngineTraceClient");
	ASSERT(g_Panel);
	ASSERT(g_Surface);
	ASSERT(g_pEntityList);
	ASSERT(g_pEngine);
	ASSERT(g_pModelInfo);
	ASSERT(g_pTrace);
	Log::Write(DEBUG, "Interfaces loaded 0x%.8X", g_pEngine);

	//Credits to lilneo/exept/null for the address class and this cool code. It looks neater and stops pointer-arithmetic from being a problem.
	Address CDLLTable = ((Address)g_pClient).To<DWORD*>();
	Address pShutdown = (CDLLTable.As<DWORD*>())[4];
	g_pClientMode = *pShutdown.GetOffset(0xF2).To<CClientMode**>();
	ASSERT(g_pClientMode);
	Log::Write(DEBUG, "g_pClientMode found 0x%.8X", g_pClientMode);

	//SigScan clientinit((DWORD*)(CDLLTable.As<DWORD*>())[0], 0x100);
	//ASSERT(g_pGlobals = **(CGlobals***)((DWORD)clientinit.Scan("\xA3", 1)));
	g_pGlobals = **(CGlobals***)(Utils::FindPattern((DWORD)GetModuleHandleA("client.dll"), 0x7CE000, (BYTE*)"\xA1\x00\x00\x00\x00\x8B\x4D\xFC\x8B\x55\x08", "x????xxxxxx") + 1);
	ASSERT(g_pGlobals);
	Log::Write(DEBUG, "g_pGlobals found 0x%.8X", g_pGlobals);

}