#ifndef VGE_VULKANQUEUEMANAGER_H
#define VGE_VULKANQUEUEMANAGER_H

#include <optional>

class VGE_VulkanManager;

struct VkPhysicalDevice_T;
struct VkQueue_T;

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


class VGE_VulkanQueueManager
{
public:
    //----------Constructors------------\\
   
    VGE_VulkanQueueManager(VGE_VulkanManager* vulkanManager);
    
    //----------Destructors------------\\
    
    ~VGE_VulkanQueueManager();

    //----------Functions------------\\

public:
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice_T* device);
    
    //bool IsComplete(QueueFamilyIndices indices);

    inline QueueStruct* GetQueues() { return &Queues; }


    //----------Variables------------\\

protected:
    QueueStruct Queues;
    VGE_VulkanManager* VulkanManager = nullptr;
};
#endif

