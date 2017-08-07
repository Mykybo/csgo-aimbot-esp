#include "InterfaceEntityList.h"
#include "InterfaceEngine.h"

CEntityList *g_pEntityList;

CBaseEntity* CEntityList::GetLocalPlayer()
{
	return GetClientEntity(g_pEngine->GetLocalPlayer());
}

std::vector<CBaseEntity*> CEntityList::GetAllPlayers()
{
	std::vector<CBaseEntity*> retn;
	for (int i = 1; i < GetHighestEntityIndex(); i++)
	{
		CBaseEntity* ent = GetClientEntity(i);
		if (!ent) continue;
		if (ent->IsPlayer())
			retn.push_back(ent);
	}
	return retn;
}

std::vector<CBaseEntity*> CEntityList::GetAllEntities()
{
	std::vector<CBaseEntity*> retn;
	for (int i = 1; i < GetHighestEntityIndex(); i++)
	{
		CBaseEntity* ent = GetClientEntity(i);
		if (!ent) continue;
		retn.push_back(ent);
	}
	return retn;
}