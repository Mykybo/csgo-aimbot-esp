#pragma once
#include "BaseInterface.h"

class CGlobals extends BaseInterface
{
public:
	float     realtime;
	int       framecount;
	float     absoluteframetime;
	float     absoluteframestarttimestddev;
	float     curtime;
	float     frametime;
	int       maxClients;
	int       tickcount;
	float     interval_per_tick;
	float     interpolation_amount;
	int       simTicksThisFrame;
	int       network_protocol;
	void*     pSaveData;
	bool      m_bClient;
	bool      m_bRemoteClient;
};

extern CGlobals* g_pGlobals;