#pragma once
#include <Windows.h>
#include "CBaseEntity.h"
#include "Utils.h"
#include "Log.h"

// #define SLEEP_TIME 1

class AntiAim
{

public:
	void Think(float elapsedtime, CUserCmd* cmd);
	void CheckControls();
	void FastSpin(CUserCmd* cmd);
	void CL_FixMove(CUserCmd* pCmd);

	static AntiAim& GetAntiAim()
	{
		static AntiAim AntiAim;
		return AntiAim;
	}

private:
	AntiAim();
	AntiAim(const AntiAim&); //do not define
	void operator=(const AntiAim&); //do not define

	bool m_Enabled;


	void Normalize(Vector &vIn, Vector &vOut)
	{
		float flLen = vIn.Length();
		if (flLen == 0) {
			vOut.Init(0, 0, 1);
			return;
		}
		flLen = 1 / flLen;
		vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
	}

	float DotProduct(const Vector& a, const Vector& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	void SinCos(float a, float* s, float*c)
	{
		*s = sin(a);
		*c = cos(a);
	}

	void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);

		if (forward)
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr*-sy);
			up->y = (cr*sp*sy + -sr*cy);
			up->z = cr*cp;
		}
	}

	Vector multiplyVector(float fl, Vector vec) {
		vec.x *= fl;
		vec.y *= fl;
		vec.z *= fl;
		return vec;
	}

};