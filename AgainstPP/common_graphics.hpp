#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <Windows.h>

#include "error.hpp"

class common_graphics
{
public:
	static egraphics_result init (HINSTANCE hInstance, HWND hWnd);
	static void exit ();

	static VkInstance instance;
	static VkPhysicalDevice physical_device;
	static VkDevice graphics_device;
	static uint32_t graphics_queue_family_index;
	static VkQueue graphics_queue;
	static VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
	static VkPhysicalDeviceLimits physical_device_limits;
	static VkSurfaceFormatKHR chosen_surface_format;
	static VkExtent2D surface_extent;
	static VkSwapchainKHR swapchain;
	static uint32_t swapchain_image_count;
	static std::vector<VkImage> swapchain_images;
	static std::vector<VkImageView> swapchain_imageviews;
	static VkSampler common_sampler;
};
