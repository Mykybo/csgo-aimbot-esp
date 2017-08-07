#pragma once
#include "BaseInterface.h"
#include "../Utils.h"

class ISurface extends BaseInterface {
public:

	void DrawSetColor(int r, int g, int b) {
		typedef void(__thiscall* oGetColor)(PVOID, int, int, int);
		return VMT::getvfunc<oGetColor>(this, 14)(this, r, g, b);
	}

	void DrawSetColor(int r, int g, int b, int a) {
		typedef void(__thiscall* oGetColor)(PVOID, int, int, int, int);
		return VMT::getvfunc<oGetColor>(this, 14)(this, r, g, b, a);
	}

	void DrawSetColor(Color col) {
		typedef void(__thiscall* oGetColor)(PVOID, Color);
		return VMT::getvfunc<oGetColor>(this, 14)(this, col);
	}

	void DrawLine(int x, int y, int xx, int yy) {
		typedef void(__thiscall* oGetLine)(PVOID, int, int, int, int);
		return VMT::getvfunc<oGetLine>(this, 19)(this, x, y, xx, yy);
	}

	void DrawFilledRect(int x, int y, int xx, int yy) {
		typedef void(__thiscall* oDrawRect)(PVOID, int, int, int, int);
		return VMT::getvfunc<oDrawRect>(this, 16)(this, x, y, xx, yy);
	}

	void DrawOutlinedRect(int x, int y, int xx, int yy) {
		typedef void(__thiscall* oGetRect)(PVOID, int, int, int, int);
		return VMT::getvfunc<oGetRect>(this, 18)(this, x, y, xx, yy);
	}

	unsigned long CreateFontA() {
		typedef unsigned long(__thiscall* oCreateFont)(PVOID);
		return VMT::getvfunc<oCreateFont>(this, 71)(this);
	}

	bool SetFontGlyphSet(unsigned long font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0) {
		typedef bool(__thiscall* oCreateFont)(PVOID, unsigned long, const char *, int, int, int, int, int, int, int);
		return VMT::getvfunc<oCreateFont>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}

	void DrawSetTextPos(int x, int y) {
		typedef void(__thiscall* oDrawSetTextPos)(PVOID, int, int);
		return VMT::getvfunc<oDrawSetTextPos>(this, 26)(this, x, y);
	}

	void DrawSetTextFont(unsigned long font) {
		typedef void(__thiscall* oDrawSetTextFont)(PVOID, unsigned long);
		return VMT::getvfunc<oDrawSetTextFont>(this, 23)(this, font);
	}

	void DrawSetTextColor(Color col) {
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, Color);
		return VMT::getvfunc<oDrawSetTextColor>(this, 24)(this, col);
	}

	//void DrawPrintText(const wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT) {
	//	typedef void(__thiscall* oDrawPrintText)(PVOID, const wchar_t*, int, FontDrawType_t);
	//	return VMT::getvfunc<oDrawPrintText>(this, 28)(this, text, textLen, drawType);
	//}

	bool GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
	{
		typedef bool(__thiscall* oGetTextSize)(PVOID, unsigned long, const wchar_t*, int&, int&);
		return VMT::getvfunc< oGetTextSize >(this, 79)(this, font, text, wide, tall);
	}

};

extern ISurface* g_Surface;
