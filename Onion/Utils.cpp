#include "Utils.h"
#include <Windows.h>
#include <iostream>
#include "Log.h"
#include "CBaseEntity.h"
#include "interfaces\InterfaceModelInfo.h"
#include "interfaces\InterfaceEntityList.h"

void Utils::InitConsole(char* name)
{
	AllocConsole();

	HWND hwnd = GetConsoleWindow();

	HMENU hMenu = GetSystemMenu(hwnd, FALSE);
	if (hMenu) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);

	SetConsoleTitle(name);
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

typedef void(__cdecl *MsgFn)(char const* pMsg, va_list);
void Utils::Msg(char * msg, ...)
{
	if (!msg)
		return;
	static MsgFn oMsg = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg");
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	oMsg(buffer, list);
}

void Utils::Error(char* msg, ...)
{
	if (!msg)
		return;
	static MsgFn oMsg = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Error");
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	oMsg(buffer, list);
}

void Utils::Warning(char* msg, ...)
{
	if (!msg)
		return;
	static MsgFn oMsg = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Warning");
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	oMsg(buffer, list);
}

bool Utils::GetBonePosition(CBaseEntity* pEntity, Vector &Hitbox, int Bone) {
	matrix3x4 MatrixArray[128];
	if (!pEntity->SetupBones2(MatrixArray, 128, 0x100, 0)) {
		return false;
	}
	Hitbox = Vector(MatrixArray[Bone][0][3], MatrixArray[Bone][1][3], MatrixArray[Bone][2][3]);
	return true;
}

Vector Utils::GetHitboxPosition(int Hitbox, CBaseEntity* ent)
{
	CBaseEntity* target = (CBaseEntity*)ent;
	void* model = target->GetModel();
	if (!model)
		return Vector(0, 0, 0);

	studiohdr_t* hdr = g_pModelInfo->GetStudioModel(model);
	if (!hdr)
		return Vector(0, 0, 0);

	VMatrix matrix[128];
	if (!target->SetupBones((matrix3x4_t*)matrix, 128, 0x100, 0))
		return Vector(0, 0, 0);

	mstudiohitboxset_t *set = hdr->pHitboxSet(target->GetHitboxSet());
	if (!set)
		return Vector(0, 0, 0);

	//std::cout << "======= Hitboxes =========" << std::endl;
	//for (int i = 0; i < set->numhitboxes; i++)
	//{
	//	mstudiobbox_t* box = set->pHitbox(i);
	//	if (!box)
	//		continue;
	//	std::cout << "Hitbox: ";
	//	std::cout << box << std::endl;
	//	std::cout << box->bone << std::endl;
	//	std::cout << box->group << std::endl;
	//}

	mstudiobbox_t* box = set->pHitbox(Hitbox);

	if (!box)
		return Vector(0, 0, 0);

	Vector center = ((box->bbmin + box->bbmax) * .5f);
	Vector hitboxpos;
	VectorTransform(center, matrix[box->bone], hitboxpos);

	return hitboxpos;
}

trace_t Utils::TraceRay(Vector start, Vector end, CBaseEntity* skipent)
{
	if (!start.IsValid() || !end.IsValid() || !(CBaseEntity*)skipent)
		return trace_t();
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = (CBaseEntity*)skipent;

	ray.Init(start, end);
	g_pTrace->TraceRay(ray, 0x4600400B, &filter, &tr);
	return tr;
}

bool Utils::IsVisible(Vector start, Vector end, CBaseEntity* skipent, CBaseEntity* target)
{
	trace_t tr = TraceRay(start, end, skipent);
	return tr.m_pEnt == target;
}

bool Utils::HitboxVisible(int hitbox, CBaseEntity* ent)
{
	Vector  hitboxpos;
	hitboxpos = Utils::GetHitboxPosition(hitbox, ent);
	return (Utils::IsVisible(g_pEntityList->GetLocalPlayer()->GetEyePosition(), hitboxpos, g_pEntityList->GetLocalPlayer(), ent));
}

std::random_device rseed;
std::mt19937 rgen = std::mt19937(rseed());
std::uniform_int_distribution<int> dist = std::uniform_int_distribution<int>(0, 10);

float Utils::RandomFloat(float a, float b)
{
	std::uniform_real_distribution<float> u(a, b);
	return u(rgen);
}

bool Utils::IsKeyDown(UINT key) {
	if (GetAsyncKeyState(key) & 0x8000) {
		return true;
	}
	return false;
}

// Press left mouse button
void Utils::StartShooting()
{
	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	}
}

// Release left mouse button
void Utils::StopShooting()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
}

bool Utils::ValidTarget(CBaseEntity* ent)
{
	if (!ent || ent->GetLifestate() != LIFE_ALIVE || ent->IsDormant() || !ent->IsPlayer() || ent->IsProtected())
		return false;
	return true;
}


bool Utils::WeaponCheck(int id)
{
	//https://tf2b.com/itemlist.php?gid=730
	// check if its grenade / knife
	if (id == 43 || id == 44 || id == 45 || id == 46 || id == 47 || id == 48 || id == 49 || id == 59 || id == 42) {
		return true;
	}
	return false;
}
