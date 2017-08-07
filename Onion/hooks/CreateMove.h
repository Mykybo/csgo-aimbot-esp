#pragma once
#include "../Vector.h"
#include "../Aimbot.h"
#include "../Radar.h"
#include "../AntiAim.h"
#include "../TriggerBot.h"
#include "../BHOP.h"
#include "../utils.h"
#include "../GlowESP.h"
#include "../interfaces/InterfaceGlobals.h"
#include "../interfaces/InterfaceEngine.h"

Aimbot& g_Aimbot = Aimbot::GetAimbot();
TriggerBot& g_TriggerBot = TriggerBot::GetTriggerBot();
Radar& g_Radar = Radar::GetRadar();
BHOP& g_BHOP = BHOP::GetBHOP();
GlowESP& g_GlowESP = GlowESP::GetGlowESP();
AntiAim& g_AntiAim = AntiAim::GetAntiAim();

bool __stdcall hkCreateMove(float frametime, CUserCmd* cmd)
{
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame()) {
		g_Aimbot.Think(g_pGlobals->absoluteframetime, cmd);
		g_TriggerBot.Think(g_pGlobals->absoluteframetime, cmd);
		g_Radar.Think(g_pGlobals->absoluteframetime);
		g_BHOP.Think(g_pGlobals->absoluteframetime, cmd);
		g_GlowESP.Think(g_pGlobals->absoluteframetime);
		// g_AntiAim.Think(g_pGlobals->absoluteframetime, cmd);
		
		// failsafe against untrusted for viewangles
		cmd->viewangles = cmd->viewangles.ClampAngle();
	}
	
	return false;
}