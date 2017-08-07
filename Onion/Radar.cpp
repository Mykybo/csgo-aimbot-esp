#include "interfaces\InterfaceEntityList.h"
#include "CBaseEntity.h"
#include "Radar.h"
#include "Utils.h"
#include "Log.h"
#include <iostream>

Radar::Radar() :
	m_Enabled(false),
	m_timerTick(0),
	m_pLocalEnt(0)
{

}

void Radar::Think(float elapsedtime)
{
	CheckControls();
	
	if (!m_Enabled)
		return;

	if (GetTickCount() < m_timerTick)
		return;

	std::vector<CBaseEntity*> entityList = g_pEntityList->GetAllPlayers();

	if (!m_pLocalEnt)
		m_pLocalEnt = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!m_pLocalEnt)
		return;

	for each (CBaseEntity* ent in entityList)
	{
		if (Utils::ValidTarget(ent) && ent->GetTeam() != m_pLocalEnt->GetTeam()) {
			if (*ent->GetSpotted() != 1) {
				*ent->GetSpotted() = 1;
			}
		}
	}

	m_timerTick = GetTickCount() + SLEEP_TIME + Utils::RandomFloat(0, 400.f);
}

void Radar::CheckControls() {
	if (GetAsyncKeyState(VK_DIVIDE) & 1)
	{
		m_Enabled = !m_Enabled;
		Log::Write(m_Enabled ? "Radar - ENABLED" : "Radar - DISABLED");
	}
}