#pragma once
#include <Windows.h>
#include "Utils.h"

#define SLEEP_TIME 25

class GlowESP
{

public:
	void Think(float elapsedtime);
	void CheckControls();
	void StartGlow(DWORD GlowPointer, int iGlow, float r, float g, float b, float a);
	void DrawGlow(CBaseEntity* Entity, DWORD dwGlowBase, int r, int g, int b, int a);
	bool GlowCheck(int id, CBaseEntity* entity, Color& clr);

	static GlowESP& GetGlowESP()
	{
		static GlowESP GlowESP;
		return GlowESP;
	}

private:
	GlowESP();
	GlowESP(const GlowESP&);
	void operator=(const GlowESP&);

	bool m_Enabled;
	bool m_OnlyVisible;
	bool m_initialized;
	DWORD m_timerTick;
	CBaseEntity* m_pLocalEnt;
	DWORD dwGlowBase;

};