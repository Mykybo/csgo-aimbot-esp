#pragma once
#include "interfaces\InterfacePanel.h"
#include "interfaces\InterfaceEngine.h"
#include "interfaces\InterfaceSurface.h"
#include "utils.h"

typedef void(__thiscall *tPaintTraverse)(void *ecx, unsigned int, bool, bool);
tPaintTraverse oPaintTraverse;

void __fastcall hkPaintTraverse(void *ecx, void *edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce) {
	// call orig paint traverse
	oPaintTraverse(ecx, vguiPanel, forceRepaint, allowForce);
	
	Log::Write(DEBUG, "hkPaintTraverse called");

}

// Paint Traverse Hooked function
//void __stdcall hkPaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
//{
//	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
//	Log::Write(DEBUG, "hkPaintTraverse called");
//
//	static unsigned int FocusOverlayPanel = 0;
//	static bool FoundPanel = false;
//
//	if (!FoundPanel)
//	{
//		Log::Write(DEBUG, "!FoundPanel");
//		PCHAR szPanelName = (PCHAR)g_Panel->GetName(vguiPanel);
//		if (strstr(szPanelName, "MatSystemTopPanel"))
//		{
//			Log::Write(DEBUG, "FOUND ONE!");
//			FocusOverlayPanel = vguiPanel;
//			FoundPanel = true;
//		}
//	}
//	else if (FocusOverlayPanel == vguiPanel)
//	{
//		Log::Write(DEBUG, "FocusOverlayPanel == vguiPanel ~ true");
//		//Render::GradientV(8, 8, 160, 18, Color(0, 0, 0, 0), Color(7, 39, 17, 255));
//		//Render::Text(10, 10, Color(255, 255, 255, 220), Render::Fonts::Menu, "PaintTraverse Works");
//		//unsigned long Font = "Arial";
//		//Render::Text(10, 10, {0,255,0,255}, Font, "PaintTraverse test!");
//
//		//Interface.Surface->SetFontGlyphSet(Font, "Verdana", 14, 0, 0, 0, FONTFLAG_ANTIALIAS);
//
//		if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
//			Log::Write(DEBUG, "In game, call draw!");
//
//		// Update and draw the menu
//		// Menu::DoUIFrame();
//	}
//}
