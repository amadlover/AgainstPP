#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Graphics
{
public:
	Graphics (HINSTANCE HInstance, HWND HWnd);
	~Graphics ();

	VkDevice GraphicsDevice;
	VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
	VkPhysicalDeviceLimits PhysicalDeviceLimits;
	VkSurfaceFormatKHR ChosenSurfaceFormat;
	VkExtent2D SurfaceExtent;
	VkSwapchainKHR Swapchain;
	std::vector<VkImage> SwapchainImages;
	std::vector<VkImageView> SwapchainImageViews;
	VkQueue GraphicsQueue;

	uint32_t GraphicsQueueFamilyIndex;
	uint32_t SwapchainImageCount;

private:

	void _PopulateInstanceLayersAndExtensions ();
	void _CreateInstance ();
	void _SetupDebugUtilsMessenger ();
	void _GetPhysicalDevice ();
	void _CreateSurface (HINSTANCE HInstance, HWND HWnd);
	void _PopulateGraphicsDeviceExtensions ();
	void _CreateGraphicsDevice ();
	void _CreateSwapChain ();
	void _CreateSwapchainImageViews ();

	bool _IsValidationNeeded;

	uint32_t RequestedInstanceLayerCount;
	const char* RequestedInstanceLayers[32];
	
	uint32_t RequestedInstanceExtensionCount;
	const char* RequestedInstanceExtensions[32];

	uint32_t RequestedDeviceExtensionCount;
	const char* RequestedDeviceExtensions[32];

	VkInstance Instance;
	VkDebugUtilsMessengerEXT DebugUtilsMessenger;
	VkPhysicalDevice PhysicalDevice;
	VkSurfaceKHR Surface;
	VkPresentModeKHR ChosenPresentMode;
};