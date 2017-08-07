#pragma once
#include "NetVar.h"
#include "VMT.h"
#include "Vector.h"
#include "Defines.h"
#include "interfaces\InterfaceClient.h"
#include "Log.h"

struct Clr {
	unsigned char r, g, b, a;
	Clr(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a)
	{

	}
};

enum EItemDefinitionIndex {
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_ump = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg556 = 39,
	weapon_ssg08 = 40,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_push = 516
};

class CBaseEntity
{
public:
	int GetTeam()
	{
		return *(int*)(this + g_NetVars.GetNetVar("DT_BaseEntity", "m_iTeamNum"));
	}

	Vector& GetAbsOrigin()
	{
		typedef Vector&(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 10)(this);
	}

	int GetHealth()
	{
		return *(int*)(this + g_NetVars.GetNetVar("DT_BasePlayer", "m_iHealth"));
	}

	unsigned int GetFlags()
	{
		return *(unsigned int*)(this + g_NetVars.GetNetVar("DT_BasePlayer", "m_fFlags"));
	}

	unsigned char GetLifestate() // not working?
	{
		return *(unsigned char*)(this + g_NetVars.GetNetVar("DT_BasePlayer", "m_lifeState"));
	}
	
	bool* GetSpotted()
	{
		return (bool*)(this + g_NetVars.GetNetVar("DT_BaseEntity", "m_bSpotted"));
	}

	HANDLE GetActiveWeapon()
	{
		return *(HANDLE*)(this + g_NetVars.GetNetVar("DT_BaseCombatCharacter", "m_hActiveWeapon"));
	}

	Vector GetEyePosition()
	{
		return GetAbsOrigin() + *(Vector*)(this + g_NetVars.GetNetVar("DT_LocalPlayerExclusive", "m_vecViewOffset[0]"));
	}

	bool SetupBones(matrix3x4_t* bone, int maxbones, int bonemask, float currenttime)
	{
		void* renderable = this + 0x4;
		typedef bool(__thiscall* fn)(void*, matrix3x4_t*, int, int, float);
		return VMT::getvfunc<fn>(renderable, 13)(renderable, bone, maxbones, bonemask, currenttime);
	}

	bool SetupBones2(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef bool(__thiscall* oSetupBones)(PVOID, matrix3x4*, int, int, float);
		return VMT::getvfunc<oSetupBones>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	void* GetModel()
	{
		void* renderable = (void*)(this + 0x4);
		typedef void*(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(renderable, 8)(renderable);
	}

	int GetIndex()
	{
		void* networkable = this + 0x8;
		typedef int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(networkable, 10)(networkable);
	}

	ClientClass* GetClientClass()
	{
		void* networkable = this + 0x8;
		typedef ClientClass*(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(networkable, 2)(networkable);
	}

	bool IsPlayer()
	{
		std::string name(GetClientClass()->m_pNetworkName);
		return (name.find("CCSPlayer") != std::string::npos);
	}
	
	// DT_BaseEntity | m_clrRender | 0x70
	Clr GetClrRender()
	{
		return *(Clr*)(this + g_NetVars.GetNetVar("DT_BaseEntity", "m_clrRender"));
	}

	// m_fImmuneToGunGameDamageTime - protection time
	// DT_CSPlayerResource | m_bHasDefuser | 0x17B8
	// DT_CSPlayer | m_bHasDefuser | 0xA908
	bool IsProtected()
	{
		return *(bool*)(this + g_NetVars.GetNetVar("DT_CSPlayer", "m_bGunGameImmunity")); // 0x38A0
	}

	float ProtectionTime()
	{
		return *(float*)(this + g_NetVars.GetNetVar("DT_CSPlayer", "m_fImmuneToGunGameDamageTime")); // 0x3898
	}

	int GetHitboxSet()
	{
		return *(int*)(this + g_NetVars.GetNetVar("DT_BaseAnimating", "m_nHitboxSet"));
	}

	int shotsfired()
	{
		return *(int*)(this + g_NetVars.GetNetVar("DT_CSLocalPlayerExclusive", "m_iShotsFired"));
	}

	Vector GetAimPunch()
	{
		return *(Vector*)(this + g_NetVars.GetNetVar("DT_LocalPlayerExclusive", "m_Local") + g_NetVars.GetNetVar("DT_Local", "m_aimPunchAngle"));
	}

	int GetCrosshairID()
	{
		// pattern: xxxxxx????????xx????xx????xxx????xx
		// 0x56, 0x57, 0x8B, 0xF9, 0xC7, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x0, 0x81, 0xF9, 0x00, 0x00, 0x00, 0x0, 0x75, 0x07, 0xA1, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x07
		// + 6
		// return *(int*)(this + 0x0000A954);
		return *(bool*)(this + g_NetVars.GetNetVar("DT_CSPlayer", "m_bHasDefuser") + 0x4C);
	}

	bool isDefusing() {
		return *(bool*)(this + g_NetVars.GetNetVar("DT_CSPlayer", "m_bIsDefusing"));
	}

	bool IsDormant()
	{
		void* networkable = (void*)(this + 0x8);
		typedef bool(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(networkable, 9)(networkable);
	}

	Vector GetVelocity()
	{
		return *(Vector*)(this + g_NetVars.GetNetVar("DT_BasePlayer", "m_vecVelocity[0]"));
	}

	int GetGlowIndex()
	{
		return *reinterpret_cast<int*>((DWORD)this + (DWORD) 0xA310);
	}

};

class CBaseCombatWeapon extends CBaseEntity
{
public:
	int* GetItemDefinitionIndex()
	{
		return (int*)(this + g_NetVars.GetNetVar("DT_BaseAttributableItem", "m_AttributeManager") + g_NetVars.GetNetVar("DT_AttributeContainer", "m_Item") + g_NetVars.GetNetVar("DT_ScriptCreatedItem", "m_iItemDefinitionIndex"));
	}

	int GetClip()
	{
		return *(int*)(this + g_NetVars.GetNetVar("DT_BaseCombatWeapon", "m_iClip1"));
	}

	bool isKnife() {
		switch (*this->GetItemDefinitionIndex()) {
		case weapon_knife:
		case weapon_knife_butterfly:
		case weapon_knife_falchion:
		case weapon_knife_flip:
		case weapon_knife_gut:
		case weapon_knife_karambit:
		case weapon_knife_m9_bayonet:
		case weapon_knife_push:
		case weapon_knife_survival_bowie:
		case weapon_knife_t:
		case weapon_knife_tactical:
			return true;
		default: 
			return false;
		}
	}

	bool isPistol() {
		switch (*this->GetItemDefinitionIndex()) {
		case weapon_cz75a:
		case weapon_deagle:
		case weapon_elite:
		case weapon_fiveseven:
		case weapon_glock:
		case weapon_hkp2000:
		case weapon_p250:
		case weapon_revolver:
		case weapon_tec9:
		case weapon_usp_silencer:
			return true;
		default:
			return false;
		}
	}

	bool isGrenade() {
		switch (*this->GetItemDefinitionIndex()) {
		case weapon_decoy:
		case weapon_flashbang:
		case weapon_hegrenade:
		case weapon_incgrenade:
		case weapon_molotov:
		case weapon_smokegrenade:
			return true;
		default:
			return false;
		}
	}

	// untested
	float GetInaccuracy() {
		DWORD dwInaccuracyVMT = (*reinterpret_cast< PDWORD_PTR* >(this))[484]; //459
		return ((float(__thiscall*)(CBaseCombatWeapon*)) dwInaccuracyVMT)(this);
	}

	float GetSpread() {
		DWORD dwSpreadVMT = (*reinterpret_cast< PDWORD_PTR* >(this))[485]; //460
		return ((float(__thiscall*)(CBaseCombatWeapon*)) dwSpreadVMT)(this);
	}

	void UpdateAccuracyPenalty() {
		DWORD dwUpdateVMT = (*reinterpret_cast< PDWORD_PTR* >(this))[486]; //461
		return ((void(__thiscall*)(CBaseCombatWeapon*)) dwUpdateVMT)(this);
	}

	/*
	// Usage - no spread
	float fRand1 = RandomFloat( 0.f, 1.f );
	float fRandPi1 = RandomFloat( 0.f, 2.f * (float) M_PI );
	float fRand2 = RandomFloat( 0.f, 1.f );
	float fRandPi2 = RandomFloat( 0.f, 2.f * (float) M_PI );

	if ( pWeapon->iGetItemDefinitionIndex() == WEAPON_REVOLVER && pCommand->buttons & IN_ATTACK2 )
	{
	fRand1 = 1.f - fRand1 * fRand1;
	fRand2 = 1.f - fRand2 * fRand2;
	}

	float fRandInaccuracy = fRand1 * pWeapon->GetInaccuracy();
	float fRandSpread = fRand2 * pWeapon->GetSpread();

	float fSpreadX = cos( fRandPi1 ) * fRandInaccuracy + cos( fRandPi2 ) * fRandSpread;
	float fSpreadY = sin( fRandPi1 ) * fRandInaccuracy + sin( fRandPi2 ) * fRandSpread;
	*/
};