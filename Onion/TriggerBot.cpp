#include "interfaces\InterfaceEntityList.h"
#include "CBaseEntity.h"
#include "TriggerBot.h"
#include "Utils.h"
#include "Log.h"
#include <iostream>

TriggerBot::TriggerBot() :
	m_Enabled(false),
	m_timerTick(0),
	m_pLocalEnt(0),
	m_shoot(false)
{

}

void TriggerBot::Think(float elapsedtime, CUserCmd* cmd)
{
	CheckControls();

	if (!m_Enabled)
		return;

	if (GetTickCount() < m_timerTick)
		return;

	m_pLocalEnt = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!m_pLocalEnt)
		return;

	CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_pEntityList->GetClientEntityFromHandle(m_pLocalEnt->GetActiveWeapon());
	if (!weapon)
		return;

	if (Utils::WeaponCheck(*weapon->GetItemDefinitionIndex())) return;

	int targetId = m_pLocalEnt->GetCrosshairID();
	if (targetId > 0 && targetId < 65) {
		CBaseEntity* ent = g_pEntityList->GetClientEntity(targetId);
		if (Utils::ValidTarget(ent) && ent->GetTeam() != m_pLocalEnt->GetTeam()) {
			if (!Utils::IsKeyDown(VK_LBUTTON) && Utils::IsKeyDown(VK_MENU)) {
				//Utils::StartShooting();
				//Utils::StopShooting();
				if (m_shoot) {
					cmd->buttons |= IN_ATTACK;
					m_shoot = false;
				}
				else {
					m_shoot = true;
					cmd->buttons &= ~IN_ATTACK;
					m_timerTick = GetTickCount() + SLEEP_TIME;
				}
			}
		}
	}

	// m_timerTick = GetTickCount() + SLEEP_TIME;
}

void TriggerBot::CheckControls() {
	if (GetAsyncKeyState(VK_NUMPAD7) & 1)
	{
		m_Enabled = !m_Enabled;
		Log::Write(m_Enabled ? "TriggerBot - ENABLED" : "TriggerBot - DISABLED");
	}
}