#include "DebugLogger.h"

#include <iostream>
#include <fstream>

DebugLogger::EDebugMessageType DebugLogger::CurrentSeverity = EDebugMessageType::NONE;
std::string DebugLogger::OutputName = "";

void DebugLogger::Initialize(const std::string& name)
{
	OutputName = name + ".txt";
	std::ofstream outFile;
	outFile.open(OutputName.c_str(), std::ios::out);
	outFile.close();
	CurrentSeverity = EDebugMessageType::INFO;
}

void DebugLogger::SetSeverity(EDebugMessageType messageType)
{
	CurrentSeverity = messageType;
}

void DebugLogger::Info(const std::string& message, const std::string& fileName, const int line)
{
	Log(EDebugMessageType::INFO, "[INFO]: " + message, fileName, line);
}

void DebugLogger::Trace(const std::string& message, const std::string& fileName, const int line)
{
	Log(EDebugMessageType::TRACE, "[TRACE]: " + message, fileName, line);
}

void DebugLogger::Warning(const std::string& message, const std::string& fileName, const int line)
{
	Log(EDebugMessageType::WARNING, "[WARNING]: " + message, fileName, line);
}

void DebugLogger::Error(const std::string& message, const std::string& fileName, const int line)
{
	Log(EDebugMessageType::ERROR, "[ERROR]: " + message, fileName, line);
}

void DebugLogger::FatalError(const std::string& message, const std::string& fileName, const int line)
{
	Log(EDebugMessageType::FATAL_ERROR, "[FATAL ERROR]: " + message, fileName, line);
}

void DebugLogger::Log(const EDebugMessageType messageType, const std::string& message, const std::string& fileName, const int line)
{
	if (CurrentSeverity > EDebugMessageType::NONE && messageType <= CurrentSeverity)
	{
		std::ofstream outFile;
		outFile.open(OutputName.c_str(), std::ios::app | std::ios::out);
		outFile << message << " in: " << fileName << " on line: " << line << std::endl;
		std::cout << message << " in: " << fileName << " on line: " << line << std::endl;
		outFile.flush();
		outFile.close();
	}
}
