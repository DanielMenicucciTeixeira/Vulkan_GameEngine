//#include "OpenGLShaderManager.h"
//#include "../Engine/Core/DebugLogger.h"
//#include <glew.h>
//#include <fstream>
//
//OpenGLShaderManager::OpenGLShaderManager()
//{
//}
//
//OpenGLShaderManager::~OpenGLShaderManager()
//{
//	CleanUp();
//}
//
//void OpenGLShaderManager::CreateShaderProgram(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
//{
//	std::string vertexShaderCode = ReadShader(vertexShaderPath);
//	std::string fragmentShaderCode = ReadShader(fragmentShaderPath);
//	if (vertexShaderCode == "" || fragmentShaderCode == "") return;
//
//	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, shaderName);
//	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, shaderName);
//	if (vertexShader == 0 || fragmentShader == 0) return;
//
//	GLint linkResult = 0;
//	GLuint program = glCreateProgram();
//	glAttachShader(program, vertexShader);
//	glAttachShader(program, fragmentShader);
//
//	glLinkProgram(program);
//	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
//
//	if (!linkResult)
//	{
//		GLint infoLogLength = 0;
//		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
//		if (infoLogLength > 0)
//		{
//			std::vector<char> programLog(infoLogLength);
//			glGetShaderInfoLog(program, infoLogLength, NULL, &programLog[0]);
//			std::string programString(programLog.begin(), programLog.end());
//			DebugLogger::Error("Error to link shader: " + shaderName + ".\nError: " + programString, "Renderers/OpenGL/OpenGLShaderManager.cpp", __LINE__);
//		}
//		else
//		{
//			DebugLogger::Error("Error to link shader: " + shaderName + ". Failed to recover info log.", "Renderers/OpenGL/OpenGLShaderManager.cpp", __LINE__);
//		}
//		glDeleteShader(vertexShader);
//		glDeleteShader(fragmentShader);
//		glDeleteProgram(program);
//		return;
//	}
//
//	Programs[shaderName] = program;
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//}
//
//unsigned int OpenGLShaderManager::GetShader(const std::string& shaderName)
//{
//	if (Programs.find(shaderName) != Programs.end()) return Programs[shaderName];
//	else return 0;
//}
//
//void OpenGLShaderManager::CleanUp()
//{
//	if (Programs.size() > 0) for (auto program : Programs) glDeleteProgram(program.second);
//	Programs.clear();
//}
//
//std::string OpenGLShaderManager::ReadShader(const std::string& filePath)
//{
//	std::string shaderCode = "";
//	std::ifstream file;
//	file.exceptions(std::ifstream::badbit);
//	try
//	{
//		file.open(filePath);
//		std::stringstream tempStream;
//		tempStream << file.rdbuf();
//		file.close();
//		shaderCode = tempStream.str();
//	}
//	catch (std::ifstream::failure error)
//	{
//		DebugLogger::Error("Failed to read shader at: " + filePath, "Renderers/OpenGL/OpenGLShaderManager.cpp", __LINE__);
//		return "";
//	}
//	return shaderCode;
//}
//
//unsigned int OpenGLShaderManager::CreateShader(unsigned int shaderType, const std::string& source, const std::string& shaderName)
//{
//	GLint compileResult = 0;
//	GLuint shader = glCreateShader(shaderType);
//	const char* shaderCode = source.c_str();
//	const int shaderCodeSize = source.size();
//
//	glShaderSource(shader, 1, &shaderCode, &shaderCodeSize);
//	glCompileShader(shader);
//
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
//
//	if (!compileResult)
//	{
//		GLint infoLogLength = 0;
//		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
//		std::vector<char> shaderLog(infoLogLength);
//		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]);
//		std::string shaderString(shaderLog.begin(), shaderLog.end());
//		DebugLogger::Error("Error compiling shader: " + shaderName + ".\nError: " + shaderString, "Renderers/OpenGL/OpenGLShaderManager.cpp", __LINE__);
//		return 0;
//	}
//	return shader;
//}
