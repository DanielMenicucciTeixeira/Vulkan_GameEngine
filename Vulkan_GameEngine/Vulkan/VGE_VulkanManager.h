#ifndef VGE_VULKANMANAGER_H
#define VGE_VULKANMANAGER_H

#include <vector>
#include <set>
#include <optional>
#include <string>

//---Forward-Declarations---\\

enum VkResult;
enum VkPresentModeKHR;
enum VkFormat;

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
struct VkImage_T;
struct VkImageView_T;
struct VkShaderModule_T;
struct VkPipelineLayout_T;
struct VkRenderPass_T;
struct VkPipeline_T;
struct VkFramebuffer_T;
struct VkCommandPool_T;
struct VkCommandBuffer_T;
struct VkSemaphore_T;

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
	void Run();
	
	inline VGE_SDLManager* GetSDLManager() { return SDLManager; }
	inline void SetSDLManager(VGE_SDLManager* sdlManager) { SDLManager = sdlManager; }
	inline SDL_Window* GetWindow() { return Window; }
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
	SDL_Window* Window = nullptr;

	//------Swap-Chain---------\\
	//Functions
protected:
	bool CheckDeviceExtensionSupport(VkPhysicalDevice_T* device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice_T* device);

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities);

	void CreateSwapChain(VkPhysicalDevice_T* physicalDevice, VkDevice_T* logicalDevice);
	void CreateImageViews();

	//Variables
protected:
	VkSwapchainKHR_T* SwapChain = nullptr;
	std::vector<const char*> DeviceExtensions;
	std::vector<VkImage_T*> SwapChainImages;
	std::vector<VkFramebuffer_T*> SwapChainFramebuffers;
	VkFormat SwapChainImageFormat;
	VkExtent2D* SwapChainExtent = nullptr;
	std::vector<VkImageView_T*> SwapChainImageViews;
	
	//-----Graphics-Pipeline----\\
	//Functions
protected:
	void CreateGraphicsPipeline();
	void CreateRenderPass();

	//Variables
protected:
	VkPipelineLayout_T* PipelineLayout = nullptr;
	VkRenderPass_T* RenderPass = nullptr;
	VkPipeline_T* GraphicsPipeline = nullptr;

	//--------Shaders--------\\
	//Functions
protected:
	static std::vector<char> ReadFile(const std::string& filename);
	VkShaderModule_T* CreateShaderModule(const std::vector<char>& code);

	//-------Buffers-------\\
	//Functions
protected:
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();

	//Variables
protected:
	VkCommandPool_T* CommandPool;
	std::vector<VkCommandBuffer_T*> CommandBuffers;

	//-----Syncronization------\\
	//Functions
protected:
	void CreateSemaphores();

	//Variables
protected:
	VkSemaphore_T* ImageAvailableSemaphore;
	VkSemaphore_T* RenderFinishedSemaphore;

	//-----Rendering------\\
	//Functions
protected:
	void DrawFrame();
};
#endif

