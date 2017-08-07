#pragma once
#include <Windows.h>
#include "Utils.h"

#define SLEEP_TIME 10

class TriggerBot
{

public:
	void Think(float elapsedtime, CUserCmd* cmd);
	void CheckControls();

	static TriggerBot& GetTriggerBot()
	{
		static TriggerBot TriggerBot;
		return TriggerBot;
	}

private:
	TriggerBot();
	TriggerBot(const TriggerBot&);
	void operator=(const TriggerBot&);

	bool m_Enabled;
	bool m_shoot;
	DWORD m_timerTick;
	CBaseEntity* m_pLocalEnt;

};