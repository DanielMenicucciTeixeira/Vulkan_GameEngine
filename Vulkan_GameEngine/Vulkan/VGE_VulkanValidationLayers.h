#ifndef VGE_VULKANVALIDATIONLAYERS_H
#define VGE_VULKANVALIDATIONLAYERS_H

#include <set>
#include<vector>

enum VkResult;
struct VkInstance_T;
struct VkDebugUtilsMessengerCreateInfoEXT;
struct VkAllocationCallbacks;
struct VkDebugUtilsMessengerEXT_T;

class VGE_VulkanValidationLayers
{
public:
    //--------Constructors---------\\

    VGE_VulkanValidationLayers();

    //--------Denstructors---------\\

    ~VGE_VulkanValidationLayers();

protected:
    bool EnableValidationLayers = false;

    std::set<const char*> ValidationLayerNames =
    {
        "VK_LAYER_KHRONOS_validation"
    };

public:
    //--------Fucntions--------\\

    inline bool IsEnabled() { return EnableValidationLayers; }
    inline std::set<const char*> GetValidationLayerNames() { return ValidationLayerNames; }
    inline void AddValidationLayer(const char* layerName) { ValidationLayerNames.insert(layerName); }
    inline void RemoveValidationLayer(const char* layerName) { ValidationLayerNames.erase(layerName); }

    bool CheckValidationLayerSupport();
    const char* const* GetData();
    std::vector<const char*> GetVector();

    VkResult CreateDebugUtilsMessengerEXT(VkInstance_T*& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT_T** pDebugMessenger);
};
#endif

