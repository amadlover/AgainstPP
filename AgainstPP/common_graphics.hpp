#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <Windows.h>

#include "error.hpp"

/*namespace common_graphics
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
}*/

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
	static VkCommandPool command_pool;
	static VkSampler common_sampler;
	
	static VkDescriptorPool imgui_descriptor_pool;
	static VkDescriptorSetLayout imgui_descriptor_set_layout;
	static VkPipelineLayout imgui_pipeline_layout;
	static VkPipeline imgui_pipeline;

	static std::vector<VkSemaphore> swapchain_signal_semaphores;
	static VkSemaphore swapchain_wait_semaphore;
	static std::vector<VkCommandBuffer> swapchain_command_buffers;

};
