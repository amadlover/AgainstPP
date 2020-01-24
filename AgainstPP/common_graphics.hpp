#pragma once

#include <vulkan.hpp>
#include <vector>
#include <Windows.h>

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
		vk::Sampler common_sampler;
	};

	void init (HINSTANCE hInstance, HWND hWnd, common_graphics* ptr);
	void exit ();
}