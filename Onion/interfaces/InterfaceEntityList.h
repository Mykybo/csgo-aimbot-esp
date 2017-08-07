#pragma once
#include "../CBaseEntity.h"
#include "BaseInterface.h"
#include <vector>

class CEntityList extends BaseInterface
{
public:
	CBaseEntity* GetClientEntity(int index)
	{
		VRESULT(CBaseEntity*, 3, Args(int))(index);
	}

	CBaseEntity* GetClientEntityFromHandle(HANDLE handle)
	{
		VRESULT(CBaseEntity*, 4, Args(HANDLE))(handle);
	}

	int GetHighestEntityIndex()
	{
		VRESULT(int, 6, Args())();
	}

	CBaseEntity* GetLocalPlayer();

	std::vector<CBaseEntity*> GetAllPlayers();
	std::vector<CBaseEntity*> GetAllEntities();

};

extern CEntityList *g_pEntityList;