#pragma once
#include <Windows.h>

#define SLEEP_TIME 500

class CBaseEntity;

class Radar
{

public:
	void Think(float elapsedtime);
	void CheckControls();

	static Radar& GetRadar()
	{
		static Radar radar;
		return radar;
	}

private:
	Radar();
	Radar(const Radar&); //do not define
	void operator=(const Radar&); //do not define

	bool m_Enabled;
	DWORD m_timerTick;
	CBaseEntity* m_pLocalEnt;

};