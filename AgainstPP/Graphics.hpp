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
	VkImage* SwapchainImages;
	std::vector<VkImageView> SwapchainImageViews;
	VkQueue GraphicsQueue;

	uint32_t GraphicsQueueFamilyIndex;

private:

	void _PopulateInstanceLayersAndExtensions ();
	void _CreateInstance ();
	void _SetupDebugUtilsMessenger ();
	void _GetPhysicalDevice ();
	void _CreateSurface (HINSTANCE HInstance, HWND HWnd);

	bool _IsValidationNeeded;

	uint32_t RequestedInstanceLayerCount;
	uint32_t RequestedInstanceExtensionCount;

	const char* RequestedInstanceLayers[32];
	const char* RequestedInstanceExtensions[32];

	const char* RequestedDeviceExtensions[32];

	VkInstance Instance;
	VkDebugUtilsMessengerEXT DebugUtilsMessenger;
	VkPhysicalDevice PhysicalDevice;
	VkSurfaceKHR Surface;
	VkPresentModeKHR ChosenPresentMode;
};