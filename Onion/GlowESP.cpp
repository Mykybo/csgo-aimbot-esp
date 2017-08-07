#pragma once
#include "interfaces\InterfaceEntityList.h"
#include "CBaseEntity.h"
#include "GlowESP.h"
#include "Utils.h"
#include "NetVar.h"
#include "Log.h"
#include <iostream>

GlowESP::GlowESP() :
	m_Enabled(true),
	m_OnlyVisible(false),
	m_timerTick(0),
	m_pLocalEnt(0),
	m_initialized(false),
	dwGlowBase(0)
{

}

void GlowESP::Think(float elapsedtime)
{
	CheckControls();

	if (!m_Enabled)
		return;

	if (GetTickCount() < m_timerTick)
		return;

	m_pLocalEnt = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!m_pLocalEnt)
		return;

	if (!m_initialized) {
		dwGlowBase = Utils::FindPattern((DWORD)GetModuleHandleA("client.dll"), 0xFFFFFF, (BYTE*)"\xA1\x00\x00\x00\x00\xA8\x01\x75\x4E\x0F\x57\xC0\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xF3\x0F\x7F\x05", "x????xxxxxxxxx????xxxxxxxx");
		m_initialized = true;
	}
	
	std::vector<CBaseEntity*> entityList = g_pEntityList->GetAllEntities();
	
	typedef uintptr_t(__stdcall* fnGetGlowObjectManager)();
	fnGetGlowObjectManager pGetObjMngr = (fnGetGlowObjectManager)dwGlowBase;
	GlowObjectDefinition* glow_object_array = *(GlowObjectDefinition**)pGetObjMngr();

	// DWORD glowObjectsCount = *(DWORD*)((DWORD)GetModuleHandleA("client.dll") + (DWORD)0x4F629FC + 0xC);

	DWORD glowObjectsCount = 600;

	//if (glowObjectsCount > 600 || glowObjectsCount < 40) {
	//	glowObjectsCount = 400;
	//}

	for (auto i = 0; i < glowObjectsCount; i++) {
		if (!glow_object_array[i].pEntity || glow_object_array[i].pEntity == 0) continue;
		CBaseEntity* glowEnt = glow_object_array[i].pEntity;
		Color glowColor = { 0, 0, 0, 180 };
		for each (CBaseEntity* ent in entityList) {
			if (ent == glow_object_array[i].pEntity) {
				if (!ent || ent->GetLifestate() || ent->IsDormant()) break;
				if (!ent->GetClientClass()) break;
				if (ent->IsPlayer()) {
					int entTeam = ent->GetTeam();
					// only enemy team
					if (m_pLocalEnt->GetTeam() == entTeam) break;
					if (m_OnlyVisible && !Utils::HitboxVisible(6, ent)) break;

					// 1 - gotv, 2 - t, 3 - ct
					// 128, 0, 128, 180 //purple
					if (entTeam == 2) {
						glowColor = { 224, 175, 86, 153 };
					}
					if (entTeam == 3) {
						glowColor = { 114, 155, 221, 153 };
					}
					if (Utils::HitboxVisible(6, ent)) {
						glowColor = { 255, 0, 102, 200 };
					}
					if (ent->IsProtected()) {
						glowColor = { 0, 255, 0, 153 };
					}
					if (ent->isDefusing()) {
						glowColor = { 128, 0, 128, 180 };
					}
				}
				else if (GlowCheck(ent->GetClientClass()->m_ClassID, ent, glowColor)) break;
				glow_object_array[i].m_bRenderWhenOccluded = !ent->IsDormant();
				glow_object_array[i].m_bRenderWhenUnoccluded = false;
				glow_object_array[i].m_bFullBloomRender = false; // 1
				glow_object_array[i].m_flRed = (1 / 255.f)*glowColor.r;
				glow_object_array[i].m_flGreen = (1 / 255.f)*glowColor.g;
				glow_object_array[i].m_flBlue = (1 / 255.f)*glowColor.b;
				glow_object_array[i].m_flAlpha = (1 / 255.f)*glowColor.a;

				// m_clrdRender
				// ent->GetClrRender() = Clr( glowColor.r, glowColor.g, glowColor.b, glowColor.a );
			}
		}
	}
	m_timerTick = GetTickCount() + SLEEP_TIME;
}

void GlowESP::DrawGlow(CBaseEntity* Entity, DWORD dwGlowBase, int r, int g, int b, int a)
{
	int iEntity_glow_index = Entity->GetGlowIndex();
	typedef uintptr_t(__stdcall* fnGetGlowObjectManager)();
	fnGetGlowObjectManager pGetObjMngr = (fnGetGlowObjectManager)dwGlowBase;
	GlowObjectDefinition* glow_object_array = *(GlowObjectDefinition**)pGetObjMngr();

	glow_object_array[iEntity_glow_index].m_bRenderWhenOccluded = !Entity->IsDormant();
	glow_object_array[iEntity_glow_index].m_bRenderWhenUnoccluded = false;
	glow_object_array[iEntity_glow_index].m_bFullBloomRender = false;
	glow_object_array[iEntity_glow_index].m_flRed = (1 / 255.f)*r; 
	glow_object_array[iEntity_glow_index].m_flGreen = (1 / 255.f)*g;
	glow_object_array[iEntity_glow_index].m_flBlue = (1 / 255.f)*b;
	glow_object_array[iEntity_glow_index].m_flAlpha = (1 / 255.f)*a;
}

void GlowESP::StartGlow(DWORD GlowPointer, int iGlow, float r, float g, float b, float a) {
	*(float*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x4) = (1 / 255.f)*r;
	*(float*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x8) = (1 / 255.f)*g;
	*(float*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0xC) = (1 / 255.f)*b;
	*(float*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x10) = (1 / 255.f)*a;

	*(bool*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x24) = true;
	*(bool*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x25) = false;
	*(bool*)(DWORD)((GlowPointer)+(iGlow * 0x38) + 0x26) = false;
}

void GlowESP::CheckControls() {
	if (GetAsyncKeyState(VK_NUMPAD4) & 1)
	{
		m_Enabled = !m_Enabled;
		Log::Write(m_Enabled ? "GlowESP - ENABLED" : "GlowESP - DISABLED");
	}
	if (m_Enabled && GetAsyncKeyState(VK_NUMPAD5) & 1) {
		m_OnlyVisible = !m_OnlyVisible;
		Log::Write(m_OnlyVisible ? "Show only visible - ENABLED" : "Show only visible - DISABLED");
	}
}

// ==============================
bool GlowESP::GlowCheck(int id, CBaseEntity* entity, Color& clr)
{
	id = (ClassID)id;
	switch (id) {
	case AK47:
	case DEagle:
	case WeaponAug:
	case WeaponAWP:
	case WeaponBizon:
	case WeaponElite:
	case WeaponFiveSeven:
	case WeaponG3SG1:
	case WeaponGalilAR:
	case WeaponGlock:
	case WeaponHKP2000:
	case WeaponM249:
	case WeaponM4A1:
	case WeaponMP7:
	case WeaponMP9:
	case WeaponMag7:
	case WeaponNOVA:
	case WeaponNegev:
	case WeaponP250:
	case WeaponP90:
	case WeaponSCAR20:
	case WeaponSG556:
	case WeaponSSG08:
	case WeaponTaser:
	case WeaponTec9:
	case WeaponUMP45:
	case WeaponXM1014:
	{
		clr = { 255, 255, 0, 150 };
		break;
	}
	case HEGrenade:
	case SmokeGrenade:
	case MolotovGrenade:
	case IncendiaryGrenade:
	case Flashbang:
	case DecoyGrenade:
	case DecoyProjectile:
	case SmokeStack:
	case MolotovProjectile:
	case SmokeGrenadeProjectile:
	case BaseCSGrenadeProjectile:
	{
		clr = { 0, 255, 0, 200 };
		break;
	}
	case Hostage:
	case Chicken:
	{
		clr = { 255, 105, 180, 150 };
		break;
	}
	case C4:
	case PlantedC4:
	{
		clr = { 199, 21, 133, 200 };
		break;
	}
	default:
		break;
	}
	return (clr.r == 0 && clr.g == 0 && clr.b == 0);
}