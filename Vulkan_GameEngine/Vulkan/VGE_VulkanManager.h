#ifndef VGE_VULKANMANAGER_H
#define VGE_VULKANMANAGER_H

#include <vector>

struct VkInstance_T;
struct VkPhysicalDevice_T;
struct VkDevice_T;

class VGE_SDLManager;
class SDL_Window;

class VGE_VulkanValidationLayers;
class VGE_VulkanDebugMessenger;
class VGE_VulkanQueueManager;
class VGE_VulkanWindowManager;
class VGE_VulkanSwapChainManager;

class VGE_VulkanManager
{
public:
	//----------Constructors------------\\
	
	VGE_VulkanManager();
	VGE_VulkanManager(VGE_SDLManager* sdlManager);

	//----------Destructors------------\\

	~VGE_VulkanManager();

	//----------Functions------------\\

public:
	void run();
	void TestMe();
	
	inline VGE_SDLManager* GetSDLManager() { return SDLManager; }
	inline void SetSDLManager(VGE_SDLManager* sdlManager) { SDLManager = sdlManager; }
	inline VGE_VulkanWindowManager* GetWindowManager() { return WindowManager; }
	inline VGE_VulkanQueueManager* GetQueueManager() { return QueueManager; }
	SDL_Window* GetWindow();

	struct VkSurfaceKHR_T* GetSurface();

protected:
    void Initialize();

    void MainLoop();

    void Cleanup();

	void CreateInstance(const char* applicationName = "App Name", const char* engineName = "Engine Name"/*, int appVersion[3] = { 1, 0, 0 }, int engineVersion[3] = { 1, 0, 0 }, int apiVersion[3] = { 1, 0, 0 }*/);

	void PickPhysicalDevice();

	bool IsPhysicalDeviceSuitable(VkPhysicalDevice_T* device);

	void CreateLogicalDevice();
	//---------Variables------------\\

	VkInstance_T* Instance = nullptr;

	VkPhysicalDevice_T* PhysicalDevice = nullptr;
	VkDevice_T* LogicalDevice = nullptr;

protected:
	VGE_SDLManager* SDLManager = nullptr;
	VGE_VulkanValidationLayers* ValidationLayers = nullptr;
	VGE_VulkanDebugMessenger* DebugMessenger = nullptr;
	VGE_VulkanQueueManager* QueueManager = nullptr;
	VGE_VulkanWindowManager* WindowManager = nullptr;
	VGE_VulkanSwapChainManager* SwapChainManager = nullptr;
};
#endif

