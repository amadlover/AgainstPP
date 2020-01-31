#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <Windows.h>

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
	common_graphics (HINSTANCE hInstance, HWND hWnd);
	~common_graphics ();

	void init ();
	void exit ();

	VkDevice graphics_device;
	uint32_t graphics_queue_family_index;
	VkQueue graphics_queue;
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
	VkPhysicalDeviceLimits physical_device_limits;
	VkSurfaceFormatKHR chosen_surface_format;
	VkExtent2D surface_extent;
	VkSwapchainKHR swapchain;
	uint32_t swapchain_image_count;
	std::vector<VkImage> swapchain_images;
	std::vector<VkImageView> swapchain_imageviews;
	VkCommandPool command_pool;
	VkSampler common_sampler;

private:
	void populate_instance_layers_and_extensions ();
	void create_instance ();
	void setup_debug_utils_messenger ();
	void get_physical_device ();
	void create_surface (HINSTANCE hInstance, HWND hWnd);
	void populate_graphics_device_extensions ();
	void create_graphics_device ();
	void create_swapchain ();
	void create_swapchain_imageviews ();
	void create_command_pool ();
	void create_sampler ();

	std::vector<const char*> requested_instance_layers;
	std::vector<const char*> requested_instance_extensions;
	std::vector<const char*> requested_device_extensions;
	
	bool is_validation_needed;
	
	VkInstance instance;
	VkDebugUtilsMessengerEXT debug_utils_messenger;
	VkPhysicalDevice physical_device;
	VkSurfaceKHR surface;
	VkPresentModeKHR chosen_present_mode;
};
