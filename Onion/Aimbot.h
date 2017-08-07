#pragma once
#include "Vector.h"
#include "Log.h"
#include "Utils.h"
#include <vector>
#include <Windows.h>
#include <random>

#define ADJUSTING_TIME 50
#define NEW_TARGET_TIME 250
#define STOP_SHOOTING_TIME 100
#define INPUT_TIME 500

class Aimbot
{
	enum State
	{
		NOTARGET,
		AIMTARGET,
		REACHLOCATION
	};

public:
	void Think(float elapsedtime, CUserCmd* cmd);
	bool FindTarget();

	Vector GetRecoil();
	void Aim(Vector targetangles);
	void DropTarget();

	void SmoothAngle(Vector& from, Vector& to, float percent);
	Vector Aimbot::VelocityComp(Vector EnemyPosition, Vector EnemyVelocity, Vector PlayerVelocity, float Distance);
	void UpdateAim(CUserCmd* cmd);
	void CheckControls();
	void Delay(int delay);
	bool IsActive();
	
	static Aimbot& GetAimbot()
	{
		static Aimbot bot;
		return bot;
	}

private:
	Aimbot();
	Aimbot(const Aimbot&); //do not define
	void operator=(const Aimbot&); //do not define
	
	bool m_Enabled;
	bool m_TriggerBot;
	bool m_AimStep;
	
	Vector m_recoil;
	Vector m_aimPoint;
	Vector m_overAim;
	Vector m_aimAngles;
	Vector m_OldAimPunch;

	CBaseEntity* m_pLocalEnt;
	CBaseEntity* m_pTarget;

	CBaseCombatWeapon* m_pWeapon;

	State m_State;

	int m_hitbox;
	DWORD m_timerTick;

	float m_curaimtime;
	float m_aimTime;
	bool m_newtarget;

	bool m_checkFov;
	bool m_autoShoot;
	bool m_smooth;
	bool m_noRecoil;
	bool m_shoot;
	float m_fovlimit;
	int m_bulletCountsAfterKill;
	char m_aimKey, m_altAimKey;
	
	int m_targetBone;
	int m_hitBoxIndex;

	float m_lastTickCount;
	
};
