#pragma once

#include <vulkan.hpp>
#include <vector>
#include <Windows.h>

class BaseGraphics
{
public:
	BaseGraphics (HINSTANCE HInstance, HWND HWnd);
	~BaseGraphics ();

	vk::UniqueDevice GraphicsDevice;
	VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
	VkPhysicalDeviceLimits PhysicalDeviceLimits;
	vk::SurfaceFormatKHR ChosenSurfaceFormat;
	vk::Extent2D SurfaceExtent;
	vk::UniqueSwapchainKHR Swapchain;
	std::vector<vk::Image> SwapchainImages;
	std::vector<vk::ImageView> SwapchainImageViews;
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

	bool _IsValidationNeeded;

	std::vector<const char*> RequestedInstanceLayers;
	std::vector<const char*> RequestedInstanceExtensions;
	std::vector<const char*> RequestedDeviceExtensions;

	//uint32_t RequestedInstanceLayerCount;
	//const char* RequestedInstanceLayers[32];
	
	/*uint32_t RequestedInstanceExtensionCount;
	const char* RequestedInstanceExtensions[32];*/

	/*uint32_t RequestedDeviceExtensionCount;
	const char* RequestedDeviceExtensions[32];*/

	vk::UniqueInstance Instance;
	vk::UniqueDebugUtilsMessengerEXT DebugUtilsMessenger;

	vk::PhysicalDevice PhysicalDevice;
	vk::UniqueSurfaceKHR Surface;
	vk::PresentModeKHR ChosenPresentMode;
};