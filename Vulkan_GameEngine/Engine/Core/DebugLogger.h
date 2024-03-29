#ifndef DEBUGLOGGER_H
#define DEBUGLOGGER_H

#include <string>


class DebugLogger
{
public:

	DebugLogger(const DebugLogger&) = delete;
	DebugLogger(DebugLogger&&) = delete;
	DebugLogger& operator =(const DebugLogger&) = delete;
	DebugLogger& operator =(DebugLogger&&) = delete;

	DebugLogger() = delete;

	enum class EDebugMessageType : unsigned short
	{
		NONE,
		FATAL_ERROR,
		ERROR,
		WARNING,
		TRACE,
		INFO
	};

	static void Initialize(const std::string& name = "Vulkan_Game_Engine_Debug_Log");
	static void SetSeverity(EDebugMessageType messageType);
	static void Info(const std::string& message, const std::string& fileName, const int line);
	static void Trace(const std::string& message, const std::string& fileName, const int line);
	static void Warning(const std::string& message, const std::string& fileName, const int line);
	static void Error(const std::string& message, const std::string& fileName, const int line);
	static void FatalError(const std::string& message, const std::string& fileName, const int line);

private:
	static void Log(const EDebugMessageType messageType, const std::string& message, const std::string& fileName, const int line);
	static EDebugMessageType CurrentSeverity;
	static std::string OutputName;
};
#endif