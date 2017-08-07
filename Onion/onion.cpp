#include "InterfaceManager.h"
#include "interfaces\InterfaceClientMode.h"
#include "interfaces\InterfacePanel.h"
#include "interfaces\InterfaceSurface.h"
#include "interfaces\InterfaceEngine.h"
#include "InterfaceManager.h"
#include "hooks\CreateMove.h"
#include "hooks\PaintTraverse.h"
#include "NetVar.h"
#include "HookManager.h"
#include "Utils.h"
#include "Log.h"
#include <ctime> //time(0)
#include <Windows.h>
#include <iostream>
#include <memory>
#include <ctime>

HMODULE g_Module;

void Start(HMODULE h)
{
	srand(time(0));
	g_Module = h;
	// Utils::InitConsole("CSGO Console");
	InterfaceManager::GetInterfaces();
	g_NetVars.Init();

	VMT* clientmodehook = new VMT(g_pClientMode);
	clientmodehook->init();
	clientmodehook->setTableHook();
	clientmodehook->hookFunction(24, hkCreateMove);

	//VMT* paintTraverseHook = new VMT(g_Panel);
	//paintTraverseHook->init();
	//paintTraverseHook->setTableHook();
	//paintTraverseHook->hookFunction(41, hkPaintTraverse);

	Log::Write("Welcome back!");
}

BOOLEAN CheckValidHardwareID(DWORD HWID)
{
	TCHAR volumeName[MAX_PATH + 1] = { 0 };
	TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
	DWORD serialNumber = 0;
	DWORD maxComponentLen = 0;
	DWORD fileSystemFlags = 0;
	if (GetVolumeInformation(
		"C:\\",
		volumeName,
		ARRAYSIZE(volumeName),
		&serialNumber,
		&maxComponentLen,
		&fileSystemFlags,
		fileSystemName,
		ARRAYSIZE(fileSystemName)) && serialNumber == HWID)
	{
		return true;
	}
	return false;
}

BOOL WINAPI DllMain(HMODULE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		// check HWID
		if (!CheckValidHardwareID(1893223237)) {
			ExitProcess(1);
		}
		// check time limit
		//time_t now = time(0);
		//if (now > 1467402183) {
		//	ExitProcess(1);
		//}
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Start, hDllHandle, 0, 0);
	}
	return 1;
}
