#pragma once
#include <Windows.h>
#include "Utils.h"

#define SLEEP_TIME 1
#define FL_ONGROUND (1<<0)

class BHOP
{

public:
	void Think(float elapsedtime, CUserCmd* cmd);
	void Jump(DWORD Input);
	void CheckControls();

	static BHOP& GetBHOP()
	{
		static BHOP bhop;
		return bhop;
	}

private:
	BHOP();
	BHOP(const BHOP&); //do not define
	void operator=(const BHOP&); //do not define

	bool m_Enabled;
	DWORD m_timerTick;
	DWORD m_input;
	CBaseEntity* m_pLocalEnt;

};