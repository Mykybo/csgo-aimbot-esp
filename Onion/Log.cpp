#include "Defines.h"
#include "Log.h"
#include <time.h>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include "Utils.h"

//rewrote katatems thing so its not shit
void Log::Write(const char* format, ...)
{
	char buffer[1024];
	va_list list;
	va_start(list, format);
	vsnprintf(buffer, 1024, format, list);
	va_end(list);
	Write(INFO, buffer);
}

void Log::Write(LogLevel level, const char* format, ...)
{
#ifdef NDEBUG
	if (level == DEBUG) return;
#endif
	char buffer[1024];
	std::string time = TimeNow();

	va_list list;
	va_start(list, format);
	vsnprintf(buffer, 1024, format, list);
	va_end(list);

	std::stringstream str;
	str << "[" << time << "] " << "[" << LevelToString(level) << "] " << buffer;
	if (level == FATAL) {
		Utils::Error((char*)str.str().c_str());
	}
#ifdef NLOGGING
	return;
#endif
	OpenFileAndAppend("log.txt", str.str());
#ifdef _CONSOLE_LOGGING
	std::stringstream().swap(str); //clears the stringstream
	str << "[" << time << "] ";
	Utils::Warning((char*)str.str().c_str());

	std::stringstream().swap(str);
	str << "[" << LevelToString(level) << "] ";
	Utils::Msg((char*)str.str().c_str());

	std::stringstream().swap(str);
	str << buffer << "\n";
	Utils::Msg((char*)str.str().c_str());
#endif
}

std::string Log::LevelToString(LogLevel level)
{
#define CASE(x) case x: return #x
	switch (level) {
		CASE(DEBUG);
		CASE(INFO);
		CASE(WARNING);
		CASE(FATAL);
	}
	return "UNK";
}

void Log::OpenFileAndAppend(std::string filename, std::string log)
{
	std::ofstream file;
	file.open(filename, std::ios_base::out | std::ios_base::app);
	file << log << std::endl;
	file.close();
}

std::string Log::TimeNow()
{
	time_t rawtime;
	struct tm* timeinfo;
	char timebuffer[128];
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d %H:%M:%S", timeinfo);

	return std::string(timebuffer);
}

