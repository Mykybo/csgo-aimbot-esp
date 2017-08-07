#pragma once
#include <iostream>

typedef void* (*CreateInterfaceFn)(const char* name, int* returncode);
class InterfaceManager
{
public:
	static void GetInterfaces();
	template<typename fn> static fn GetInterface(std::string modulename, std::string interfacename);
};
