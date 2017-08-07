#include "interfaces\InterfaceEntityList.h"
#include "AntiAim.h"

AntiAim::AntiAim() :
	m_Enabled(false)
{

}

void AntiAim::Think(float elapsedtime, CUserCmd* cmd)
{
	CheckControls();

	if (!m_Enabled)
		return;
	
	CBaseEntity* m_pLocalEnt = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!m_pLocalEnt || m_pLocalEnt->GetLifestate() != LIFE_ALIVE)
		return;

	if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_USE)
		return;

	CL_FixMove(cmd);

}

// Yaws
void AntiAim::FastSpin(CUserCmd *pCmd)
{
	int spinSpeed = 20;
	Vector viewangles = pCmd->viewangles;
	static float current_y = viewangles.y;
	current_y += spinSpeed;
	viewangles.y = current_y;
	pCmd->viewangles = viewangles.ClampAngle();
}

void BackJitter(CUserCmd *pCmd)
{
	int random = rand() % 100;
	Vector viewangles = pCmd->viewangles;
	float change = 180 + Utils::RandomFloat(-5, 5);
	viewangles.y += change;
	viewangles.x = 89 - Utils::RandomFloat(0, 5);
	pCmd->viewangles = viewangles.ClampAngle();
}

void AntiAim::CL_FixMove(CUserCmd* pCmd)
{
	// Backup for safety
	Vector origView = pCmd->viewangles;
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
	Vector qAimAngles;
	qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
	AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

	// FastSpin(pCmd);
	BackJitter(pCmd);

	//Movement Fix
	AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
	qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
	AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
	Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
	Vector vRightNorm;			Normalize(viewright, vRightNorm);
	Vector vUpNorm;				Normalize(viewup, vUpNorm);

	// Original shit for movement correction
	float forward = pCmd->forwardmove;
	float right = pCmd->sidemove;
	float up = pCmd->upmove;
	if (forward > 450) forward = 450;
	if (right > 450) right = 450;
	if (up > 450) up = 450;
	if (forward < -450) forward = -450;
	if (right < -450) right = -450;
	if (up < -450) up = -450;
	pCmd->forwardmove = DotProduct(multiplyVector(forward, vForwardNorm), aimforward) + DotProduct(multiplyVector(right, vRightNorm), aimforward) + DotProduct(multiplyVector(up, vUpNorm), aimforward);
	pCmd->sidemove = DotProduct(multiplyVector(forward, vForwardNorm), aimright) + DotProduct(multiplyVector(right, vRightNorm), aimright) + DotProduct(multiplyVector(up, vUpNorm), aimright);
	pCmd->upmove = DotProduct(multiplyVector(forward, vForwardNorm), aimup) + DotProduct(multiplyVector(right, vRightNorm), aimup) + DotProduct(multiplyVector(up, vUpNorm), aimup);
}

//bool EdgeAntiAim(CUserCmd *cmd, float flWall, float flCornor)
//{
//	CBaseEntity* localPlayer = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
//	static CTraceFilterNoPlayers filter;
//
//	bool ret = false;
//
//	Vector localPosition = localPlayer->GetEyePosition();
//
//	for (int y = 0; y < 360; y++)
//	{
//		Vector tmp(10.0f, cmd->viewangles.y, 0.0f);
//		tmp.y += y;
//		tmp.ClampAngle();
//
//		Vector forward;
//		AngleVectors(tmp, forward);
//
//		float length = ((16.0f + 3.0f) + ((16.0f + 3.0f) * sin(DegToRad(10.0f)))) + 7.0f;
//		forward *= length;
//
//		Ray_t ray;
//		trace_t traceData;
//
//		ray.Init(localPosition, (localPosition + forward));
//		Utils::TraceRay(ray, 0x200400B, (CTraceFilter*)&filter, &traceData);
//
//		if (traceData.fraction != 1.0f)
//		{
//			Vector angles;
//			Vector Negate = traceData.plane.normal;
//
//			Negate *= -1;
//			VectorAngles(Negate, angles);
//
//			tmp.y = angles.y;
//			tmp.ClampAngle();
//			trace_t leftTrace, rightTrace;
//
//			Vector left, right;
//			AngleVectors(tmp + Vector(0.0f, 30.0f, 0.0f), left);
//			AngleVectors(tmp - Vector(0.0f, 30.0f, 0.0f), right);
//
//			left *= (length + (length * sin(DegToRad(30.0f))));
//			right *= (length + (length * sin(DegToRad(30.0f))));
//
//			ray.Init(localPosition, (localPosition + left));
//			Utils::TraceRay(ray, 0x200400B, (CTraceFilter*)&filter, &leftTrace);
//
//			ray.Init(localPosition, (localPosition + right));
//			Utils::TraceRay(ray, 0x200400B, (CTraceFilter*)&filter, &rightTrace);
//
//			if ((leftTrace.fraction == 1.0f)
//				&& (rightTrace.fraction != 1.0f))
//			{
//				tmp.y -= flCornor;
//				// LEFT
//			}
//			else if ((leftTrace.fraction != 1.0f)
//				&& (rightTrace.fraction == 1.0f))
//			{
//				tmp.y += flCornor;
//				// RIGHT
//			}
//
//			cmd->viewangles.y = tmp.y;
//			cmd->viewangles.y -= flWall;
//			ret = true;
//		}
//	}
//
//	return ret;
//}

/*
bool bEdge = EdgeAntiAim( pLocal, pCmd, 360.0f, 45.0f );
if( bEdge ) let the magic happen w/e
*/

void AntiAim::CheckControls() {
	if (GetAsyncKeyState(VK_F4) & 1)
	{
		m_Enabled = !m_Enabled;
		Log::Write(m_Enabled ? "AntiAim - ENABLED" : "AntiAim - DISABLED");
	}
}