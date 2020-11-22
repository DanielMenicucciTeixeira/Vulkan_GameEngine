#ifndef VULKANPIPELINE_H
#define VULKANPIPELINE_H

class VulkanManager;

class VulkanPipeline
{
public:
	VulkanPipeline(VulkanManager* manager);

protected:
	VulkanManager* Manager = nullptr;
};
#endif

