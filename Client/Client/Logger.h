#pragma once
#include <fstream>
class Logger
{
public:

	Logger() noexcept;
	~Logger() noexcept;
	void write(std::string);
private:
	std::string path = "log.txt";
	std::ofstream logFile;
};

