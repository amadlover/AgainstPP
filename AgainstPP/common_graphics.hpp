#pragma once

#include <vulkan.hpp>
#include <vector>
#include <Windows.h>

class BaseGraphics
{
public:
	BaseGraphics (HINSTANCE HInstance, HWND HWnd);
	~BaseGraphics ();

	vk::Device GraphicsDevice;
	vk::PhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
	vk::PhysicalDeviceLimits PhysicalDeviceLimits;
	vk::SurfaceFormatKHR ChosenSurfaceFormat;
	vk::Extent2D SurfaceExtent;
	vk::SwapchainKHR Swapchain;
	std::vector<vk::Image> SwapchainImages;
	std::vector<vk::ImageView> SwapchainImageViews;
	vk::Queue GraphicsQueue;

	std::vector<uint32_t> GraphicsQueueFamilies;

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

	vk::Instance Instance;
	vk::DebugUtilsMessengerEXT DebugUtilsMessenger;

	vk::PhysicalDevice PhysicalDevice;
	vk::SurfaceKHR Surface;
	vk::PresentModeKHR ChosenPresentMode;
};

namespace common_graphics
{
	struct common_graphics
	{
		vk::Device graphics_device;
		std::vector<uint32_t> graphics_queue_family_indices;
		vk::Queue graphics_queue;
		vk::PhysicalDeviceMemoryProperties physical_device_memory_properties;
		vk::PhysicalDeviceLimits physical_device_limits;
		vk::SurfaceFormatKHR chosen_surface_format;
		vk::Extent2D surface_extent;
		vk::SwapchainKHR swapchain;
		std::vector<vk::Image> swapchain_images;
		std::vector<vk::ImageView> swapchain_imageviews;
		vk::CommandPool command_pool;
	};

	void init (HINSTANCE hInstance, HWND hWnd, common_graphics* ptr);
	void exit ();
}