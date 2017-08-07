#pragma once
#include "BaseInterface.h"

class IPanel extends BaseInterface {
public:

	const char* GetName(unsigned int vguiPanel) {
		typedef const char*(__thiscall* oGetName)(PVOID, unsigned int);
		return VMT::getvfunc<oGetName>(this, 36)(this, vguiPanel);
	}

};

extern IPanel* g_Panel;
