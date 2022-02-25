//#ifndef OPENGLSHADERMANAGER_H
//#define OPENGLSHADERMANAGER_H
//
//#include <unordered_map>
//#include <sstream>
//#include <vector>
//
//class OpenGLShaderManager
//{
//public:
//	OpenGLShaderManager(const OpenGLShaderManager&) = delete;
//	OpenGLShaderManager(OpenGLShaderManager&&) = delete;
//	OpenGLShaderManager& operator=(const OpenGLShaderManager&) = delete;
//	OpenGLShaderManager& operator=(OpenGLShaderManager&&) = delete;
//
//	OpenGLShaderManager();
//	~OpenGLShaderManager();
//	
//	void CreateShaderProgram(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
//	unsigned int GetShader(const std::string& shaderName);
//	void CleanUp();
//
//protected:
//	std::string ReadShader(const std::string& filePath);
//	unsigned int CreateShader(unsigned int shaderType, const std::string& source, const std::string& shaderName);
//	std::unordered_map<std::string, unsigned> Programs;
//
//};
//#endif
//
