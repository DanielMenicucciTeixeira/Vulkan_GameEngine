#ifndef VGE_VULKANMANAGER_H
#define VGE_VULKANMANAGER_H

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

private:
    void initVulkan();

    void mainLoop();

    void cleanup();
};
#endif

