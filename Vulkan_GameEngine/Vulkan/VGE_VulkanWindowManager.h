#ifndef VGE_VULKANWINDOWMANAGER_H
#define VGE_VULKANWINDOWMANAGER_H

struct VkSurfaceKHR_T;
struct VkInstance_T;
class SDL_Window;

class VGE_VulkanWindowManager
{
public:
	//----------Constructors------------\\
	
	VGE_VulkanWindowManager();
	VGE_VulkanWindowManager(SDL_Window* window);

	//----------Destructors------------\\
	
	~VGE_VulkanWindowManager();

	//----------Functions------------\\

	void CreateSurface(VkInstance_T* instance);
	void CreateSurface(VkInstance_T* instance, SDL_Window* window);
	inline void SetWindow(SDL_Window* window) { Window = window; }
	inline VkSurfaceKHR_T* GetSurface() { return Surface; }
	SDL_Window* GetWindow();

	//---------Variables------------\\

protected:
	VkSurfaceKHR_T* Surface = nullptr;
	SDL_Window* Window;

};
#endif

