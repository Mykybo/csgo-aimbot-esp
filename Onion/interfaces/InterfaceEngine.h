#pragma once
#include "BaseInterface.h"
#include "../Vector.h"

typedef struct player_info_s {
private:
	DWORD __pad0[2];
public:
	int m_nXuidLow;
	int m_nXuidHigh;
	char m_szPlayerName[128];
	int m_nUserID;
	char m_szSteamID[33];
	UINT m_nSteam3ID;
	char m_szFriendsName[128];
	bool m_bIsFakePlayer;
	bool m_bIsHLTV;
	DWORD m_dwCustomFiles[4];
	BYTE m_FilesDownloaded;
private:
	int __pad1;
} player_info_t;

struct VMatrix
{
public:
	float* operator[](int i) { return m[i]; }
	const float* operator[](int i) const { return m[i]; }
	float *Base() { return &m[0][0]; }
	const float *Base() const { return &m[0][0]; }

	float	m[4][4];
};

class CEngine extends BaseInterface 
{
public:
	void GetScreenSize(int& width, int& height)
	{
		VCALL(5, Args(int&, int&))(width, height);
	}

	bool GetPlayerInfo(int index, player_info_t* info)
	{
		VRESULT(bool, 8, Args(int, player_info_t*))(index, info);
	}

	int GetLocalPlayer()
	{
		VRESULT(int, 12, Args())();
	}

	void GetViewAngles(Vector& va) { VCALL(18, Args(Vector&))(va); }
	void SetViewAngles(Vector& va) { VCALL(19, Args(Vector&))(va); }

	VMatrix WorldToScreenMatrix()
	{
		//defines don't work for this
		typedef VMatrix(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 37)(this);
	}

	bool IsConnected()
	{
		typedef bool(__thiscall* oGetScreenSize)(PVOID);
		return VMT::getvfunc< oGetScreenSize >(this, 27)(this);
	}

	bool IsInGame()
	{
		typedef bool(__thiscall* oLocal)(PVOID);
		return VMT::getvfunc< oLocal >(this, 26)(this);
	}
};

extern CEngine* g_pEngine;