#ifndef VGE_VULKANMANAGER_H
#define VGE_VULKANMANAGER_H

#include <vector>

struct VkInstance_T;

class VGE_SDLManager;

class VGE_VulkanManager
{
public:
	//----------Constructors------------\\
	
	VGE_VulkanManager();

	//----------Destructors------------\\

	~VGE_VulkanManager();

	//----------Functions------------\\

public:
	void run();
	void TestMe();
	inline VGE_SDLManager* GetSDLManager() { return SDLManager; }
	inline void SetSDLManager(VGE_SDLManager* sdlManager) { SDLManager = sdlManager; }

protected:
    void Initialize();

    void MainLoop();

    void Cleanup();

	void CreateInstance(const char* applicationName = "App Name", const char* engineName = "Engine Name"/*, int appVersion[3] = { 1, 0, 0 }, int engineVersion[3] = { 1, 0, 0 }, int apiVersion[3] = { 1, 0, 0 }*/);

	//---------Variables------------\\

	VkInstance_T* Instance = nullptr;

protected:
	VGE_SDLManager* SDLManager = nullptr;
	class VGE_VulkanValidationLayers* ValidationLayers = nullptr;
	class VGE_VulkanDebugMessenger* DebugMessenger = nullptr;

};
#endif

