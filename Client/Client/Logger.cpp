#define _CRT_SECURE_NO_WARNINGS
#include "Logger.h"
#include <chrono>

Logger::Logger() noexcept
{
	logFile.open(path.c_str(), std::ios::out);
	write("start working");
}

Logger::~Logger() noexcept
{
	logFile.close();
}

void Logger::write(std::string text)
{
	auto nowTime = std::chrono::system_clock::now();
	auto tmp = std::chrono::system_clock::to_time_t(nowTime);
	logFile << std::ctime(&tmp) << " " << text << std::endl;
}
