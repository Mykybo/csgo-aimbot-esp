#pragma once
#include <string>

enum LogLevel
{
	DEBUG,
	INFO,
	WARNING,
	FATAL
};

class Log
{
public:
	static void Write(const char* format, ...);
	static void Write(LogLevel level, const char* format, ...);
	static std::string LevelToString(LogLevel level);
	static void OpenFileAndAppend(std::string filename, std::string log);
	static std::string TimeNow();
	
};