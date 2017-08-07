#include "HookManager.h"
#include "Log.h"
#include <Windows.h>

HookManager::HookManager(void* object)
{
	base = static_cast<DWORD**>(object);
	originalVMT = *base;

	methodCount = GetMethodCount();
	modifiedVMT = new DWORD[methodCount];

	Log::Write(DEBUG, "modifiedVMT is 0x%.8X", modifiedVMT);
	Log::Write(DEBUG, "originalVMT is 0x%.8X", originalVMT);
	Log::Write(DEBUG, "methodCount is");
	Log::Write(DEBUG, std::to_string(methodCount).c_str());
	return;
	memcpy(modifiedVMT, originalVMT, methodCount * sizeof(DWORD));

	*base = modifiedVMT;
}

HookManager::~HookManager()
{
	delete modifiedVMT;
}

DWORD HookManager::HookMethod(int index, DWORD function)
{
	if (index < methodCount)
	{
		modifiedVMT[index] = function;

		return originalVMT[index];
	}

	return 0;
}

DWORD HookManager::GetOriginalFunction(int index) const
{
	return originalVMT[index];
}

void HookManager::Unhook()
{
	*base = originalVMT;
}

int HookManager::GetMethodCount() const
{
	DWORD index = 0;
	for (index; originalVMT[index]; index++)
	{
		if (IsBadCodePtr((FARPROC)originalVMT[index]))
		{
			break;
		}
	}

	return index;
}
