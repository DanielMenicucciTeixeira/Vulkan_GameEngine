#ifndef VGE_VULKANMANAGER_H
#define VGE_VULKANMANAGER_H

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

private:
    void Initialize();

    void MainLoop();

    void Cleanup();

	unsigned defaultVersion[3] = { 1, 0, 0 };

	void CreateInstance(unsigned appVersion[3], unsigned engineVersion[3], unsigned apiVersion[3], const char* applicationName = "App Name", const char* engineName = "Engine Name");
	void CreateInstance(const char* applicationName = "App Name", const char* engineName = "Engine Name");

	//---------Variables------------\\

	VkInstance_T* Instance = nullptr;

protected:
	VGE_SDLManager* SDLManager = nullptr;

};
#endif

