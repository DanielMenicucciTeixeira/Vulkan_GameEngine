#ifndef VGE_VULKANMANAGER_H
#define VGE_VULKANMANAGER_H

#include <vector>
#include <set>
#include <optional>

//---Forward-Declarations---\\

enum VkResult;
enum VkPresentModeKHR;

struct VkInstance_T;
struct VkPhysicalDevice_T;
struct VkDevice_T;
struct VkQueue_T;
struct VkSurfaceKHR_T;
struct VkDebugUtilsMessengerCreateInfoEXT;
struct VkAllocationCallbacks;
struct VkDebugUtilsMessengerEXT_T;
struct VkSurfaceCapabilitiesKHR;
struct VkSurfaceFormatKHR;
struct VkExtent2D;
struct VkSwapchainKHR_T;

class VGE_SDLManager;
class SDL_Window;

//------Structs------\\

struct QueueFamilyIndices
{
	std::optional<unsigned int> GraphicsFamily;
	std::optional<unsigned int> PresentationFamily;

	inline bool IsComplete()
	{
		return GraphicsFamily.has_value()
			&& PresentationFamily.has_value();
	}
};

struct QueueStruct
{
	VkQueue_T* GraphicsQueue = nullptr;
	VkQueue_T* PresentationQueue = nullptr;
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR* Capabilities = InitializeCapabilities();
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentationModes;

	//This function makes sure Capabilities is not a nullptr, this is necessary since Capabilites cannot be properly initilized in the header while using forward declarations.
	VkSurfaceCapabilitiesKHR* InitializeCapabilities();
};

//------Class------\\

class VGE_VulkanManager
{
public:
	//Constructors
	VGE_VulkanManager(VGE_SDLManager* sdlManager = nullptr);

	//Destructors

	~VGE_VulkanManager();

	//Functions

public:
	void run();
	void TestMe();
	
	inline VGE_SDLManager* GetSDLManager() { return SDLManager; }
	inline void SetSDLManager(VGE_SDLManager* sdlManager) { SDLManager = sdlManager; }
	//inline SDL_Window* GetWindow() { return Window; }
	inline VkSurfaceKHR_T* GetSurface() { return Surface; };

protected:
    void Initialize();
    void MainLoop();
    void Cleanup();

	void CreateInstance(const char* applicationName = "App Name", const char* engineName = "Engine Name"/*, int appVersion[3] = { 1, 0, 0 }, int engineVersion[3] = { 1, 0, 0 }, int apiVersion[3] = { 1, 0, 0 }*/);
	void PickPhysicalDevice();
	bool IsPhysicalDeviceSuitable(VkPhysicalDevice_T* device);
	void CreateLogicalDevice();

	//Variables

	VkInstance_T* Instance = nullptr;

	VkPhysicalDevice_T* PhysicalDevice = nullptr;
	VkDevice_T* LogicalDevice = nullptr;

	//---Validation-Layers------\\
	//Fucntions
protected:
	bool CheckValidationLayerSupport();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance_T*& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT_T** pDebugMessenger);

	//Variables
protected:
	bool EnableValidationLayers = false;

	std::vector<const char*> ValidationLayerNames =
	{
		"VK_LAYER_KHRONOS_validation"
	};

protected:
	VGE_SDLManager* SDLManager = nullptr;

	//--------Debug-Messenger-------\\
	//Functions
protected:
	void SetUpDebugMessenger();
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void DestroyDebugUtilsMessengerEXT(VkInstance_T* instance, const VkAllocationCallbacks* pAllocator);

	//Variables
protected:
	VkDebugUtilsMessengerEXT_T* DebugMessenger = nullptr;

	//---------Queues------------\\
	//Functions
protected:
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice_T* physicalDevice);

	//Variables
protected:
	QueueStruct Queues;

	//------Window-and-Surface------\\
	//Functions
protected:
	void CreateSurface();
	inline void SetWindow(SDL_Window* window) { Window = window; }

	//Variables
protected:
	VkSurfaceKHR_T* Surface = nullptr;
	SDL_Window* Window;

	//------Swap-Chain---------\\
	//Functions
protected:
	bool CheckDeviceExtensionSupport(VkPhysicalDevice_T* device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice_T* device);

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities);

	void CreateSwapChain(VkPhysicalDevice_T* physicalDevice, VkDevice_T* logicalDevice);

	//Variables
protected:
	VkSwapchainKHR_T* SwapChain = nullptr;
	std::vector<const char*> DeviceExtensions;
};
#endif

