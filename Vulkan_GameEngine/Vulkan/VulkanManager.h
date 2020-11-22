#ifndef VULKANMANAGER_H
#define VULKANMANAGER_H

#include "Renderer.h"
#include <vector>
#include <optional>

//-----Forward Declarations------\\
//Vulkan Renderer Classes
class VulkanDebugger;
class VulkanDevices;
class VulkanPipeline;
class VulkanSwapchain;

//Vulkan API Declarations
enum VkPresentModeKHR;

struct VkInstance_T;
struct VkSurfaceKHR_T;
struct VkQueue_T;
struct VkDevice_T;
struct VkPhysicalDevice_T;
struct VkDeviceMemory_T;
struct VkSurfaceCapabilitiesKHR;
struct VkSurfaceFormatKHR;
struct VkCommandBuffer_T;
struct VkCommandPool_T;
struct VkBuffer_T;

typedef unsigned int VkFlags;
typedef VkFlags VkMemoryPropertyFlags;
typedef VkFlags VkBufferUsageFlags;
typedef unsigned long long VkDeviceSize;

//SDL Classes
class SDL_Window;
class VGE_SDLManager;

//--------------------------------------------------

struct QueueStruct
{
	VkQueue_T* GraphicsQueue = nullptr;
	VkQueue_T* PresentationQueue = nullptr;
};

struct QueueFamilyIndices
{
	std::optional<unsigned int> GraphicsFamily;
	std::optional<unsigned int> PresentationFamily;

	inline bool IsComplete()
	{
		return	GraphicsFamily.has_value() &&
			PresentationFamily.has_value();
	}
};

struct SwapchainSupportDetails
{
	VkSurfaceCapabilitiesKHR* Capabilities = InitializeCapabilities02();
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentationModes;

	//This function makes sure Capabilities is not a nullptr, this is necessary since Capabilites cannot be properly initilized in the header while using forward declarations.
	VkSurfaceCapabilitiesKHR* InitializeCapabilities02();
};

class VulkanManager : public Renderer
{
//Functions
public:
	VulkanManager(VGE_SDLManager* windowManager);
	~VulkanManager();
	
	void Run() override;
	void FramebufferResizeCallback() override;
	void Render(SDL_Window** windowArray = nullptr, unsigned int numberOfWindows = 1, unsigned int arrayOffset = 0) override;

	//Getters
	inline VGE_SDLManager* GetWindowManager() { return WindowManager; }
	inline SDL_Window* GetWindow() { return Window; }
	inline VulkanDebugger* GetDebugger() { return Debugger; }
	inline VulkanDevices* GetDevices() { return Devices; }
	inline VulkanPipeline* GetGraphicsPipeline() { return GraphicsPipeline; }
	inline VulkanSwapchain* GetSwapchain() { return Swapchain; }
	inline VkInstance_T* GetInstance() { return Instance; }
	inline VkSurfaceKHR_T* GetSurface() { return Surface; }
	inline QueueStruct* GetQueues() { return Queues; }
	VkDevice_T* GetLogicalDevice();
	VkPhysicalDevice_T* GetPhysicalDevice();
	SwapchainSupportDetails GetSwapchainSupportDetails();


	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice_T* physicalDevice);
	unsigned int FindMemoryType(unsigned int typeFilter, VkMemoryPropertyFlags properties);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer_T*& buffer, VkDeviceMemory_T*& bufferMemory);

protected:
//CommandBuffers
	void CreateCommandPool();
	VkCommandPool_T* CommandPool;

public:
	VkCommandBuffer_T* BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer_T* commandBuffer);

protected:
	void Initialize() override;
	void CleanUp() override;
	SDL_Window* CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY) override;
	void CreateSurface();

	//appVersion must be an array of 3 unsigned int values, if a smaller array is passed, program might crash or appVersion might have unpredicted values
	void CreateInstance(const char* applicationName = "App Name", const char* engineName = "Engine Name", unsigned int* appVersion = nullptr);

//Variables
protected:
	VulkanDebugger* Debugger = nullptr;
	VulkanDevices* Devices = nullptr;
	VulkanPipeline* GraphicsPipeline = nullptr;
	VulkanSwapchain* Swapchain = nullptr;
	VGE_SDLManager* WindowManager = nullptr;

	SDL_Window* Window;
	VkSurfaceKHR_T* Surface = nullptr;

	VkInstance_T* Instance = nullptr;
	QueueStruct* Queues = nullptr;
};
#endif

