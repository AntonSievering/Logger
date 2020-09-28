#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

class Logger
{
	using clock_t = std::chrono::system_clock;
	using time_t  = std::chrono::time_point<clock_t>;

private:
	bool m_bFileLogging = false;
	bool m_bCommandLineLogging = false;
	bool m_bTimeStamping = false;
	std::ofstream file;
	time_t m_tStart;

public:
	Logger();
	Logger(const std::string &sFilename, bool bFileLogging = true, bool bCommandLineLogging = true, bool bTimeStamping = true);
	~Logger();

public:
	void Log(const std::string &string, const std::string &branch = "");
};

