#include "interfaces\InterfaceEntityList.h"
#include "CBaseEntity.h"
#include "BHOP.h"
#include "Utils.h"
#include "Log.h"

BHOP::BHOP() :
	m_Enabled(true),
	m_timerTick(0),
	m_pLocalEnt(0),
	m_input(MapVirtualKeyEx(VK_SPACE, MAPVK_VK_TO_VSC_EX, 0))
{

}

void BHOP::Think(float elapsedtime, CUserCmd* cmd)
{
	CheckControls();

	if (!m_Enabled)
		return;

	if (GetTickCount() < m_timerTick)
		return;

	m_pLocalEnt = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!m_pLocalEnt)
		return;

	unsigned int flags = m_pLocalEnt->GetFlags();

	if (GetAsyncKeyState(VK_XBUTTON1) < 0) { // & 0x8000
		if (flags & FL_ONGROUND) {
			// Jump(m_input);
			cmd->buttons |= IN_JUMP; 
		} else {
			cmd->buttons &= ~IN_JUMP;
		}
	}

	// teleport, no need to do the findPattern every tick!!
	// static DWORD dwbufFrameblaNilget = Utils::FindPattern((DWORD)GetModuleHandleA("client.dll"), 0xFFFFFF, (BYTE*)"\xBE\x00\x00\x00\x00\x2B\xF1\x3B\xF3\x7D\x1F\x8B\x55\x0C\x8B\xC3", "x????xxxxxxxxxxx");
	// static bool doOnce = false;
	// bool bTeleport = false;
	//if (GetAsyncKeyState('C'))
	//if (GetAsyncKeyState(VK_F6))
	//	bTeleport = TRUE;

	//if (bTeleport)
	//{
	//	if (!doOnce)
	//	{
	//		cmd->viewangles.y = -178;
	//		cmd->viewangles.x = 0;
	//		cmd->viewangles.z = 0;
	//		Vector vec;
	//		vec.y = -178;
	//		vec.x = 0;
	//		vec.z = 0;
	//		g_pEngine->SetViewAngles(vec);
	//		DWORD dwBack = NULL;
	//		VirtualProtect((LPVOID)dwbufFrameblaNilget, 4, PAGE_READWRITE, &dwBack);
	//		BYTE bBytes[] = { 0xBE, 0x1 }; // 2BC
	//		memcpy((LPVOID)dwbufFrameblaNilget, bBytes, sizeof(bBytes));
	//		VirtualProtect((LPVOID)dwbufFrameblaNilget, 4, dwBack, &dwBack);
	//		doOnce = true;
	//	}
	//}
	//else
	//{
	//	if (doOnce)
	//	{
	//		cmd->viewangles.y = -178;
	//		cmd->viewangles.x = 0;
	//		cmd->viewangles.z = 0;
	//		Vector vec;
	//		vec.y = -178;
	//		vec.x = 0;
	//		vec.z = 0;
	//		g_pEngine->SetViewAngles(vec);
	//		DWORD dwBack = NULL;
	//		VirtualProtect((LPVOID)dwbufFrameblaNilget, 4, PAGE_READWRITE, &dwBack);
	//		BYTE bBytes[] = { 0xBE, 0x20 }; // 2BC
	//		memcpy((LPVOID)dwbufFrameblaNilget, bBytes, sizeof(bBytes));
	//		VirtualProtect((LPVOID)dwbufFrameblaNilget, 4, dwBack, &dwBack);
	//		doOnce = false;
	//	}
	//}

	// another teleport
	//if (GetAsyncKeyState(VK_F7)) // KEY TO TELEPORT
	//{
	//	pCmd->viewangles.y = 22249223532141231232143.949962143f;
	//	pCmd->viewangles.x = 22249223523413423454322345.9499621213f;
	//}

	//m_timerTick = GetTickCount() + SLEEP_TIME + Utils::RandomFloat(0, 2);
}

void BHOP::Jump(DWORD Input)
{
	keybd_event(VK_LCONTROL, MapVirtualKeyEx(VK_LCONTROL, MAPVK_VK_TO_VSC_EX, 0), KEYEVENTF_SCANCODE, 0);
	keybd_event(VK_SPACE, Input, KEYEVENTF_SCANCODE, 0);
	keybd_event(VK_SPACE, Input, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LCONTROL, MapVirtualKeyEx(VK_LCONTROL, MAPVK_VK_TO_VSC_EX, 0), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
}

void BHOP::CheckControls() {
	if (GetAsyncKeyState(VK_NUMPAD1) & 1)
	{
		m_Enabled = !m_Enabled;
		Log::Write(m_Enabled ? "BHOP - ENABLED" : "BHOP - DISABLED");
	}
}