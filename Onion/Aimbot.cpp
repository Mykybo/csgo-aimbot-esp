#include "interfaces\InterfaceEntityList.h"
#include "interfaces\InterfaceEngine.h"
#include "CBaseEntity.h"
#include "Aimbot.h"
#include "Log.h"
#include "Utils.h"
#include <iostream>

Aimbot::Aimbot() :
	m_Enabled(false),
	m_TriggerBot(false),
	m_AimStep(true),
	m_recoil(0, 0, 0),
	m_aimPoint(0, 0, 0),
	m_overAim(0, 0, 0),
	m_aimAngles(0, 0, 0),
	m_pTarget(0),
	m_pLocalEnt(0),
	m_State(NOTARGET),
	m_hitbox(-1),
	m_timerTick(0),
	m_smooth(true),
	m_noRecoil(false),
	m_fovlimit(12.f), // fov for the Aimbot
	m_bulletCountsAfterKill(2),
	m_aimKey(VK_LBUTTON),
	m_altAimKey(VK_MENU),
	m_aimTime(10.f), // 4000
	m_curaimtime(0.f),
	m_newtarget(false),
	m_targetBone(6),
	m_lastTickCount(GetTickCount()),
	m_autoShoot(false),
	m_checkFov(true),
	m_pWeapon(0),
	m_shoot(false),
	m_OldAimPunch(0,0,0)
{

}

bool Aimbot::FindTarget()
{
	DropTarget();

	if (*m_pWeapon->GetItemDefinitionIndex() == weapon_awp && !Utils::IsKeyDown(m_altAimKey)) {
		return false;
	}

	float bestfov = FLT_MAX;
	
	if (m_checkFov) {
		bestfov = m_fovlimit;
	}
	Vector viewangles;
	g_pEngine->GetViewAngles(viewangles);

	std::vector<CBaseEntity*> entityList = g_pEntityList->GetAllPlayers();
	for each (CBaseEntity* ent in entityList)
	{
		if (ent->GetTeam() == m_pLocalEnt->GetTeam()) continue;
		if (!Utils::ValidTarget(ent)) continue;

		if (!Utils::HitboxVisible(6, ent) && !Utils::HitboxVisible(3, ent)) continue;
		Vector angdelta = viewangles - (ent->GetAbsOrigin() - m_pLocalEnt->GetAbsOrigin()).Angle().ClampAngle();
		angdelta.y += 180;
		if (angdelta.y > 360) angdelta.y = fmodf(angdelta.y, 360);
		angdelta.y -= 180;
		float fov = angdelta.Length2D();
		if (fov > bestfov) continue;
		// float dist = m_pLocalEnt->GetAbsOrigin().DistTo(ent->GetAbsOrigin());
		if (fov < bestfov) {
			bestfov = fov;
			m_pTarget = ent;
		}
	}
	if (m_pTarget) {
		if (Utils::HitboxVisible(m_targetBone, m_pTarget)) {
			m_hitbox = m_targetBone;
		}
		else {
			m_hitbox = m_targetBone == 6 ? 3 : 6;
		}
		return true;
	}
	return false;
}

bool Aimbot::IsActive() {
	return m_State != NOTARGET;
}

Vector Aimbot::GetRecoil()
{
	float multiplier_x = 2.f;
	float multiplier_y = 2.f;
	if (!m_noRecoil) {
		if (!m_pLocalEnt->shotsfired() > 1) return Vector(0, 0, 0);
		multiplier_x = Utils::RandomFloat(1.90f, 1.98f);
		multiplier_y = Utils::RandomFloat(1.88f, 1.95f);
	}
	Vector punchAngles = m_pLocalEnt->GetAimPunch();
	punchAngles.x *= multiplier_x;
	punchAngles.y *= multiplier_y;

	// do correction stuff
	if (m_noRecoil && m_State == NOTARGET) {
		Vector currentPunch = punchAngles;
		punchAngles -= m_OldAimPunch;
		m_OldAimPunch = currentPunch;
	}
	return punchAngles;
}

void Aimbot::DropTarget()
{
	m_State = NOTARGET;
	m_pTarget = 0;
	m_curaimtime = 0.f;
}

void Aimbot::Think(float elapsedtime, CUserCmd* cmd)
{
	// hack because getting globalvars interface fails
	//elapsedtime = GetTickCount() - m_lastTickCount;
	//m_lastTickCount = GetTickCount();
	
	CheckControls();

	if (!m_Enabled)
		return;

	if (!Utils::IsKeyDown(m_aimKey) && !Utils::IsKeyDown(m_altAimKey) && !m_autoShoot)
	{
		DropTarget();
		m_OldAimPunch.x = m_OldAimPunch.y = m_OldAimPunch.z = 0;
		return;
	}

	m_pLocalEnt = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!m_pLocalEnt)
		return;

	m_pWeapon = (CBaseCombatWeapon*)g_pEntityList->GetClientEntityFromHandle(m_pLocalEnt->GetActiveWeapon());
	if (!m_pWeapon)
		return;

	//if (Utils::WeaponCheck(*m_pWeapon->GetItemDefinitionIndex())) return;
	if (m_pWeapon->isKnife() || m_pWeapon->isGrenade()) return;

	// no ammo
	if (m_pWeapon->GetClip() == 0) {
		Utils::StopShooting();
		//cmd->buttons &= ~IN_ATTACK;
		//cmd->buttons |= IN_RELOAD;
		return;
	}

	// TODO: fixme
	//if (m_noRecoil && m_State == NOTARGET) {
	//	if (cmd->buttons & IN_ATTACK) {
	//		Vector currentang;
	//		g_pEngine->GetViewAngles(currentang);
	//		currentang = currentang - GetRecoil();
	//		g_pEngine->SetViewAngles(currentang.ClampAngle());
	//	}
	//}

	if (m_newtarget)
		m_curaimtime = 0;

	// class timer
	if (GetTickCount() < m_timerTick)
		return;

	if (m_State == NOTARGET) {
		if (FindTarget()) {
			m_State = REACHLOCATION;
			//if (m_pLocalEnt->GetAbsOrigin().DistTo(m_pTarget->GetAbsOrigin()) < 150)

			// do overaim
			if (m_smooth) {
				Vector BoneFrom;
				Utils::GetBonePosition(m_pTarget, BoneFrom, m_hitbox);
				Vector newAngles = (BoneFrom - m_pLocalEnt->GetEyePosition()).Angle().ClampAngle();
				Vector viewangles;
				g_pEngine->GetViewAngles(viewangles);
				Vector angdelta = (viewangles - newAngles).ClampAngle();
				m_overAim = angdelta * Utils::RandomFloat(0.15, 0.38);;
			}
		}
		else {
			return;
		}
	}

	switch (m_State) {
	case REACHLOCATION:
		m_newtarget = false;
		if (!Utils::ValidTarget(m_pTarget) || !Utils::HitboxVisible(m_hitbox, m_pTarget)) {
			DropTarget();
			m_newtarget = true;
			Delay(NEW_TARGET_TIME);
			return;
		}
		break;
	case AIMTARGET:
		if (!Utils::ValidTarget(m_pTarget) || !Utils::HitboxVisible(m_hitbox, m_pTarget)) {
			DropTarget();
			Delay(NEW_TARGET_TIME);
			m_newtarget = true;
			return;
		}		
		break;
	default:
		break;
	}
	 
	UpdateAim(cmd);

	m_curaimtime += elapsedtime;
	if (m_curaimtime > m_aimTime)
		m_curaimtime = m_aimTime;

	if (m_pTarget != m_pLocalEnt)
		Aim(m_aimAngles - GetRecoil());
}

void Aimbot::Delay(int delay) {
	if (!m_autoShoot) {
		m_timerTick = GetTickCount() + delay;
	}
}

void Aimbot::Aim(Vector ang)
{
	if (m_smooth) {
		Vector currentang;
		g_pEngine->GetViewAngles(currentang);
		SmoothAngle(currentang, ang, m_curaimtime / m_aimTime);
	}
	g_pEngine->SetViewAngles(ang.ClampAngle());
}

void Aimbot::UpdateAim(CUserCmd* cmd) {
	Vector BoneFrom;
	Utils::GetBonePosition(m_pTarget, BoneFrom, m_hitbox);
	Vector newAngles = (BoneFrom - m_pLocalEnt->GetEyePosition()).Angle().ClampAngle();

	Vector viewangles;
	g_pEngine->GetViewAngles(viewangles);
	Vector angdelta = (viewangles - newAngles).ClampAngle();

	if (m_smooth && m_State == REACHLOCATION) {
		newAngles -= m_overAim;
	}

	// for to target player
	float realFov = (angdelta + GetRecoil()).Length2D();

	// rewrite the angledelta after adjusting for overaim if smooth was on
	angdelta = (viewangles - newAngles).ClampAngle();

	// fov for overaim
	float fov = (angdelta + GetRecoil()).Length2D();

	if (m_State == REACHLOCATION && fov < 1.f) {
		m_State = AIMTARGET;
	}

	if ((m_autoShoot || m_TriggerBot) && realFov < 0.32f && m_State == AIMTARGET) {
		//Utils::StartShooting();
		// kill velocity
		float vel = m_pLocalEnt->GetVelocity().Length();
		if (vel <= 60.f)
		{
			cmd->sidemove = 0.f;
			cmd->forwardmove = 0.f;
		}
		else
		{
			cmd->sidemove = -cmd->sidemove;
			cmd->forwardmove = -cmd->forwardmove;
		}
		if (m_pWeapon->isPistol()) {
			if (m_shoot) {
				cmd->buttons |= IN_ATTACK;
				m_shoot = false;
			}
			else {
				cmd->buttons &= ~IN_ATTACK;
				m_shoot = true;
			}
		}
		else {
			cmd->buttons |= IN_ATTACK;
		}
	}

	if (m_AimStep) {
		float stepSize = 39.0f;
		float length = angdelta.Length2D();
		if (length > stepSize) {
			angdelta.x = (angdelta.x / length) * stepSize;
			angdelta.y = (angdelta.y / length) * stepSize;
		}
		newAngles = viewangles - angdelta;
	}

	m_aimAngles = newAngles.ClampAngle();
}

void Aimbot::SmoothAngle(Vector& from, Vector& to, float percent)
{
	Vector delta = from - to;
	delta = delta.ClampAngle();
	delta.x *= percent;
	if (percent != 1.f && percent != 0) {
		percent = percent / Utils::RandomFloat(1.5, 2.5);
	}
	delta.y *= percent;
	to = from - delta;
}

void Aimbot::CheckControls() {
	
	if (GetAsyncKeyState(VK_MULTIPLY) & 1)
	{
		m_Enabled = !m_Enabled;
		Log::Write(m_Enabled ? "Aimbot - ENABLED" : "Aimbot - DISABLED");
	}

	if (GetAsyncKeyState(VK_NUMPAD2) & 1)
	{
		m_autoShoot = !m_autoShoot;
		Log::Write(m_autoShoot ? "m_autoShoot - ENABLED" : "m_autoShoot - DISABLED");
		// if (!m_autoShoot) Utils::StopShooting();
	}

	if (GetAsyncKeyState(VK_F3) & 1)
	{
		m_checkFov = !m_checkFov;
		Log::Write(m_checkFov ? "Check FOV - ENABLED" : "Check FOV - DISABLED");
	}
	
	// target body part toggle
	if (GetAsyncKeyState(VK_NUMPAD6) & 1)
	{
		m_targetBone = 6; // head
		Log::Write("Now targeting HEAD");
	}
	if (GetAsyncKeyState(VK_NUMPAD3) & 1)
	{
		m_targetBone = 3; // chest
		Log::Write("Now targeting CHEST");
	}

	if (GetAsyncKeyState(VK_ADD) & 1)
	{
		m_aimTime += 0.5;
		Log::Write("Aim time set to:");
		Log::Write(std::to_string(m_aimTime).c_str());
	}
	if (GetAsyncKeyState(VK_SUBTRACT) & 1)
	{
		m_aimTime -= 0.5;
		Log::Write("Aim time set to:");
		Log::Write(std::to_string(m_aimTime).c_str());
	}

	// smooth toggle
	if (GetAsyncKeyState(VK_NUMPAD9) & 1)
	{
		m_smooth = !m_smooth;
		Log::Write(m_smooth ? "Smooth set to TRUE" : "Smooth set to FALSE");
	}
	// perfect recoil toggle
	if (GetAsyncKeyState(VK_NUMPAD8) & 1)
	{
		m_noRecoil = !m_noRecoil;
		Log::Write(m_noRecoil ? "Perfect NoRecoil - ENABLED" : "Perfect NoRecoil - DISABLED");
	}

	if (GetAsyncKeyState(VK_UP) & 1)
	{
		m_TriggerBot = !m_TriggerBot;
		Log::Write(m_TriggerBot ? "m_TriggerBot - ENABLED" : "m_TriggerBot - DISABLED");
	}

	if (GetAsyncKeyState(VK_DOWN) & 1)
	{
		m_AimStep = !m_AimStep;
		Log::Write(m_AimStep ? "m_AimStep - ENABLED" : "m_AimStep - DISABLED");
	}
	
}

Vector Aimbot::VelocityComp(Vector EnemyPosition, Vector EnemyVelocity, Vector PlayerVelocity, float Distance)
{
	EnemyPosition += EnemyVelocity / Distance;
	EnemyPosition -= PlayerVelocity / Distance;
	return EnemyPosition;
}
