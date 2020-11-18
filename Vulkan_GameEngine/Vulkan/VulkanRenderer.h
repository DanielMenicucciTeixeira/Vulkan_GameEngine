#ifndef VGE_VULKANMANAGER_H
#define VGE_VULKANMANAGER_H

#include "Renderer.h"

#include <vector>
#include <set>
#include <optional>
#include <string>
#include <array>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

//---Forward-Declarations---\\

enum VkResult;
enum VkPresentModeKHR;
enum VkFormat;
enum VkImageTiling;
enum VkImageLayout;

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
struct VkDescriptorSetLayout_T;
struct VkPipelineLayout_T;
struct VkRenderPass_T;
struct VkPipeline_T;
struct VkFramebuffer_T;
struct VkCommandPool_T;
struct VkCommandBuffer_T;
struct VkBuffer_T;
struct VkSemaphore_T;
struct VkFence_T;
struct VkVertexInputBindingDescription;
struct VkVertexInputAttributeDescription;
struct VkDeviceMemory_T;
struct VkDescriptorPool_T;
struct VkDescriptorSet_T;
struct VkSampler_T;


typedef unsigned int VkFlags;
typedef VkFlags VkMemoryPropertyFlags;
typedef VkFlags VkBufferUsageFlags;
typedef VkFlags VkMemoryPropertyFlags;
typedef VkFlags VkImageUsageFlags;
typedef VkFlags VkFormatFeatureFlags;
typedef VkFlags VkImageAspectFlags;
typedef unsigned long long VkDeviceSize;


class VGE_SDLManager;
class SDL_Window;
class FVector3;
class FVector4;

//------Structs------\\

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Colour;
	glm::vec2 TextureCoordinates;

	static VkVertexInputBindingDescription GetBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();

	bool operator==(const Vertex& other) const;

	/*FVector3* Position;
	FVector4* Colour;
	Vertex();
	Vertex(FVector3 position, FVector4 colour, glm::vec2 textureCoordinates);
	inline ~Vertex() {}*/
};

namespace std 
{
	template<> struct hash<Vertex> 
	{
		size_t operator()(Vertex const& vertex) const 
		{
			return ((hash<glm::vec3>()(vertex.Position) ^
				(hash<glm::vec3>()(vertex.Colour) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.TextureCoordinates) << 1);
		}
	};
}

struct UniformBufferObject 
{
	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Projection;
};

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

class VulkanRenderer : public Renderer
{
public:
	//Constructors
	VulkanRenderer(VGE_SDLManager* sdlManager = nullptr);

	//Destructors

	~VulkanRenderer();

	//Functions

public:
	void Run() override;
	
	inline VGE_SDLManager* GetSDLManager() { return SDLManager; }
	inline void SetSDLManager(VGE_SDLManager* sdlManager) { SDLManager = sdlManager; }
	inline SDL_Window* GetWindow() { return Window; }
	inline VkSurfaceKHR_T* GetSurface() { return Surface; };

protected:
    void Initialize() override;
    void MainLoop();
    void CleanUp() override;
	SDL_Window* CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY) override;

	void CreateInstance(const char* applicationName = "App Name", const char* engineName = "Engine Name"/*, int appVersion[3] = { 1, 0, 0 }, int engineVersion[3] = { 1, 0, 0 }, int apiVersion[3] = { 1, 0, 0 }*/);
	void PickPhysicalDevice();
	bool IsPhysicalDeviceSuitable(VkPhysicalDevice_T* device);
	void CreateLogicalDevice();

	//Variables
protected:
	VkInstance_T* Instance = nullptr;
	VkPhysicalDevice_T* PhysicalDevice = nullptr;
	VkDevice_T* LogicalDevice = nullptr;
	const unsigned int MAX_FRAMES_IN_FLIGHT = 2;

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
	void SetWindow(SDL_Window* window);

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

	void CreateSwapChain();
	void RecreateSwapChain();
	void CreateImageViews();
	VkImageView_T* CreateImageView(VkImage_T* image, VkFormat format, VkImageAspectFlags aspectFlags);
	void CleanUpSwapChain();

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
	VkDescriptorSetLayout_T* DescriptorSetLayout;
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
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer_T*& buffer, VkDeviceMemory_T*& bufferMemory);
	void CreateFramebuffers();
	void CreateVertexBuffers();
	void CreateIndexBuffer();
	void CreateUniformBuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	VkCommandBuffer_T* BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer_T* commandBuffer);

	void CopyBuffer(VkBuffer_T* srcBuffer, VkBuffer_T* dstBuffer, VkDeviceSize size);
	unsigned int FindMemoryType(unsigned int typeFilter, VkMemoryPropertyFlags properties);
	void UpdateUniformBuffer(unsigned int currentImageIndex);

public:
	void FramebufferResizeCallback() override;

	//Variables
protected:
	VkCommandPool_T* CommandPool;
	std::vector<VkCommandBuffer_T*> CommandBuffers;
	
	VkBuffer_T* VertexBuffer;
	VkDeviceMemory_T* VertexBufferMemory;
	
	VkBuffer_T* IndexBuffer;
	VkDeviceMemory_T* IndexBufferMemory;

	std::vector<VkBuffer_T*> UniformBuffers;
	std::vector<VkDeviceMemory_T*> UniformBuffersMemory;

	bool FramebufferResized = false;

	//-----Syncronization------\\
	//Functions
protected:
	void CreateSyncObjects();

	//Variables
protected:
	std::vector<VkSemaphore_T*> ImageAvailableSemaphores;
	std::vector<VkSemaphore_T*> RenderFinishedSemaphores;
	std::vector<VkFence_T*> InFlightFences;
	std::vector<VkFence_T*> ImagesInFlight;
	size_t CurrentFrame = 0;
	

	//-----Rendering------\\
	//Functions
protected:
	void Render() override;

	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateImage(unsigned int width, unsigned int height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage_T*& image, VkDeviceMemory_T*& imageMemory);
	void TransitionImageLayout(VkImage_T* image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer_T* buffer, VkImage_T* image, unsigned int width, unsigned int height);
	void CreateTextureSampler();
	void CreateDepthResources();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat format);

	//Variables
protected:
	VkDescriptorPool_T* DescriptorPool = nullptr;
	std::vector<VkDescriptorSet_T*> DescriptorSets;
	VkImage_T* TextureImage = nullptr;
	VkImageView_T* TextureImageView = nullptr;
	VkDeviceMemory_T* TextureImageMemory = nullptr;
	VkSampler_T* TextureSampler = nullptr;
	VkImage_T* DepthImage;
	VkDeviceMemory_T* DepthImageMemory;
	VkImageView_T* DepthImageView;

	void LoadModel();
	const std::string MODEL_PATH = "Models/viking_room.obj";
	const std::string TEXTURE_PATH = "Textures/viking_room.png";
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
};
#endif