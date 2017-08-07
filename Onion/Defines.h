#pragma once
//Took some of this stuff from katatem because I liked his macros.

#define Args(...) void* (__thiscall*)(void*, ##__VA_ARGS__)
#define extends : public

#define _CONSOLE_LOGGING
//#define NDEBUG
/* NDEBUG
This stops debug messages from being logged/printed to console if you opt for that.*/
//#define NLOGGING 
/* NLOGGING
This could be used for a legit-mode cheat so nothing prints to console.
No matter what, fatal errors will still crash the game and pop up a message box.*/

#ifndef NDEBUG
	#define ASSERT(cond) if(!(cond)) {Log::Write(FATAL, "\""#cond"\" failed.");}
#else
	#define ASSERT(cond) cond
#endif