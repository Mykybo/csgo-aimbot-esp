#pragma once
#include <Windows.h>
class HookManager
{
public:
	HookManager(void* object);
	~HookManager();

	DWORD HookMethod(int index, DWORD function);
	DWORD GetOriginalFunction(int index) const;

	void Unhook();

private:
	DWORD** base;

	DWORD* originalVMT;
	DWORD* modifiedVMT;

	int methodCount;
	int GetMethodCount() const;
};
