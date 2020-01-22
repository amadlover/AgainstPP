#include <Windows.h>

#include "graphics_utils.hpp"
#include "common_graphics.hpp"
#include "Error.hpp"

#include <vulkan/vulkan_win32.h>

PFN_vkCreateDebugUtilsMessengerEXT pfn_vk_create_debug_utils_messenger_ext;
PFN_vkDestroyDebugUtilsMessengerEXT pfn_vk_destroy_debug_utils_messenger_ext;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT (VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
	return pfn_vk_create_debug_utils_messenger_ext (instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT (VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks const* pAllocator)
{
	return pfn_vk_destroy_debug_utils_messenger_ext (instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_messenger_callback (VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (pCallbackData)
	{
		wchar_t buff[2048];
		swprintf_s (buff, 2048, L"Debug Messenger Callback: %hs\n", pCallbackData->pMessage);
		OutputDebugString (buff);
	}

	return VK_FALSE;
}

namespace common_graphics
{
	common_graphics* common_graphics_obj_ptr;

	bool is_validataion_needed = false;

	std::vector<const char*> requested_instance_layers;
	std::vector<const char*> requested_instance_extensions;
	std::vector<const char*> requested_device_extensions;

	vk::PresentModeKHR chosen_present_mode;
	vk::SurfaceKHR surface;
	vk::PhysicalDevice physical_device;
	vk::DebugUtilsMessengerEXT debug_utils_messenger;
	vk::Instance instance;

	void populate_instance_layers_and_extensions ()
	{
		if (is_validataion_needed)
		{
			std::vector<vk::LayerProperties> layer_properties = vk::enumerateInstanceLayerProperties ();
			std::vector<vk::LayerProperties>::iterator it = std::find_if (
				layer_properties.begin (),
				layer_properties.end (),
				[](const vk::LayerProperties& layer_property) {
					return strcmp (layer_property.layerName, "VK_LAYER_LUNARG_standard_validation") == 0;
				}
			);

			if (it != layer_properties.end ())
			{
				requested_instance_layers.push_back ("VK_LAYER_LUNARG_standard_validation");
			}
		}

		std::vector<vk::ExtensionProperties> extension_properties = vk::enumerateInstanceExtensionProperties ();
		std::vector<vk::ExtensionProperties>::iterator it = std::find_if (
			extension_properties.begin (),
			extension_properties.end (),
			[](const vk::ExtensionProperties& extension_property) {
				return strcmp (extension_property.extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0;
			}
		);

		if (it != extension_properties.end ())
		{
			requested_instance_extensions.push_back (VK_KHR_SURFACE_EXTENSION_NAME);
		}

		it = std::find_if (
			extension_properties.begin (),
			extension_properties.end (),
			[](const vk::ExtensionProperties& extension_property) {
				return strcmp (extension_property.extensionName, "VK_KHR_win32_surface") == 0;
			}
		);

		if (it != extension_properties.end ())
		{
			requested_instance_extensions.push_back ("VK_KHR_win32_surface");
		}

		if (is_validataion_needed)
		{
			it = std::find_if (
				extension_properties.begin (),
				extension_properties.end (),
				[](const vk::ExtensionProperties& extension_property) {
					return strcmp (extension_property.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0;
				}
			);
			requested_instance_extensions.push_back (VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
	}

	void create_instance ()
	{
		vk::ApplicationInfo AI ("Against", 1, "Against", 1, VK_API_VERSION_1_1);
		vk::InstanceCreateInfo instance_create_info (
			{},
			&AI,
			requested_instance_layers.size (),
			requested_instance_layers.data (),
			requested_instance_extensions.size (),
			requested_instance_extensions.data ()
		);

		instance = vk::createInstance (instance_create_info);
	}

	void setup_debug_utils_messenger ()
	{
		pfn_vk_create_debug_utils_messenger_ext = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr ("vkCreateDebugUtilsMessengerEXT"));
		pfn_vk_destroy_debug_utils_messenger_ext = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr ("vkDestroyDebugUtilsMessengerEXT"));

		vk::DebugUtilsMessageSeverityFlagsEXT severityFlags (
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
		/*vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);*/

		vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags (
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
			vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
			vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
		);

		debug_utils_messenger = instance.createDebugUtilsMessengerEXT (
			vk::DebugUtilsMessengerCreateInfoEXT (
				{},
				severityFlags,
				messageTypeFlags,
				&debug_messenger_callback
			)
		);
	}

	void get_physical_device ()
	{
		physical_device = instance.enumeratePhysicalDevices ().front ();

		uint32_t i = 0;
		for (auto queue_family_property : physical_device.getQueueFamilyProperties ())
		{
			if ((queue_family_property.queueFlags & vk::QueueFlagBits::eGraphics) && queue_family_property.queueCount > 1)
			{
				common_graphics_obj_ptr->graphics_queue_family_indices.push_back (i);
				break;
			}
			++i;
		}

		common_graphics_obj_ptr->physical_device_limits = physical_device.getProperties ().limits;
		common_graphics_obj_ptr->physical_device_memory_properties = physical_device.getMemoryProperties ();
	}

	void create_surface (HINSTANCE hInstance, HWND hWnd)
	{
		surface = instance.createWin32SurfaceKHR (
			vk::Win32SurfaceCreateInfoKHR ({},
				hInstance,
				hWnd
			)
		);
	}

	void populate_graphics_device_extensions ()
	{
		std::vector<vk::ExtensionProperties> extension_properties = physical_device.enumerateDeviceExtensionProperties ();
		std::vector< vk::ExtensionProperties>::iterator It = std::find_if (
			extension_properties.begin (),
			extension_properties.end (),
			[](const vk::ExtensionProperties& ExtensionProperty) {
				return strcmp (ExtensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0;
			}
		);

		requested_device_extensions.push_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	void create_graphics_device_and_queue ()
	{
		float priorities = 1.f;

		vk::DeviceQueueCreateInfo queue_create_info (
			{},
			common_graphics_obj_ptr->graphics_queue_family_indices[0],
			1,
			&priorities
		);
		vk::DeviceCreateInfo device_create_info (
			{},
			1,
			&queue_create_info,
			0,
			NULL,
			requested_device_extensions.size (),
			requested_device_extensions.data ()
		);

		common_graphics_obj_ptr->graphics_device = physical_device.createDevice (device_create_info);
		common_graphics_obj_ptr->graphics_queue = common_graphics_obj_ptr->graphics_device.getQueue (common_graphics_obj_ptr->graphics_queue_family_indices[0], 0);
	}

	void create_swapchain ()
	{
		if (!physical_device.getSurfaceSupportKHR (common_graphics_obj_ptr->graphics_queue_family_indices[0], surface))
		{
			throw egraphics_error::surface_support;
		}

		for (auto SurfaceFormat : physical_device.getSurfaceFormatsKHR (surface))
		{
			if (SurfaceFormat.format == vk::Format::eB8G8R8A8Unorm)
			{
				common_graphics_obj_ptr->chosen_surface_format = SurfaceFormat.format;
				break;
			}
		}

		for (auto present_mode : physical_device.getSurfacePresentModesKHR (surface))
		{
			if (present_mode == vk::PresentModeKHR::eMailbox)
			{
				chosen_present_mode = present_mode;
				break;
			}
		}

		vk::SurfaceCapabilitiesKHR surface_capabilities = physical_device.getSurfaceCapabilitiesKHR (surface);
		common_graphics_obj_ptr->surface_extent = surface_capabilities.currentExtent;

		vk::SwapchainCreateInfoKHR swapchain_create_info (
			{},
			surface,
			surface_capabilities.minImageCount + 1,
			common_graphics_obj_ptr->chosen_surface_format.format,
			common_graphics_obj_ptr->chosen_surface_format.colorSpace,
			common_graphics_obj_ptr->surface_extent,
			1,
			surface_capabilities.supportedUsageFlags,
			vk::SharingMode::eExclusive,
			1,
			& common_graphics_obj_ptr->graphics_queue_family_indices[0],
			surface_capabilities.currentTransform,
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			chosen_present_mode,
			VK_TRUE,
			VK_NULL_HANDLE
		);

		common_graphics_obj_ptr->swapchain = common_graphics_obj_ptr->graphics_device.createSwapchainKHR (swapchain_create_info);
		common_graphics_obj_ptr->swapchain_images = common_graphics_obj_ptr->graphics_device.getSwapchainImagesKHR (common_graphics_obj_ptr->swapchain);

		vk::ComponentMapping components (vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
		vk::ImageSubresourceRange subresource_range (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

		vk::ImageViewCreateInfo swapchain_image_view_create_info (
			{},
			nullptr,
			vk::ImageViewType::e2D,
			common_graphics_obj_ptr->chosen_surface_format.format,
			components,
			subresource_range
		);

		for (auto swapchain_image : common_graphics_obj_ptr->swapchain_images)
		{
			swapchain_image_view_create_info.image = swapchain_image;
			common_graphics_obj_ptr->swapchain_imageviews.push_back (common_graphics_obj_ptr->graphics_device.createImageView (swapchain_image_view_create_info));
		}
	}

	void create_command_pool ()
	{
		vk::CommandPoolCreateInfo command_pool_create_info ({}, common_graphics_obj_ptr->graphics_queue_family_indices[0]);
		common_graphics_obj_ptr->command_pool = common_graphics_obj_ptr->graphics_device.createCommandPool (command_pool_create_info);
	}
	
	void create_sampler ()
	{
		vk::SamplerCreateInfo sampler_create_info (
			{},
			vk::Filter::eLinear,
			vk::Filter::eLinear,
			vk::SamplerMipmapMode::eLinear,
			vk::SamplerAddressMode::eClampToBorder,
			vk::SamplerAddressMode::eClampToBorder,
			vk::SamplerAddressMode::eClampToBorder,
			0,
			0,
			0,
			0,
			{},
			0,
			0,
			vk::BorderColor::eFloatOpaqueBlack,
			0
		);

		common_graphics_obj_ptr->common_sampler = common_graphics_obj_ptr->graphics_device.createSampler (sampler_create_info);
	}

	void init (HINSTANCE hInstance, HWND hWnd, common_graphics* ptr)
	{
		OutputDebugString (L"common_graphics::init\n");

		common_graphics_obj_ptr = ptr;

#ifdef _DEBUG
		is_validataion_needed = true;
#endif

		populate_instance_layers_and_extensions ();
		create_instance ();

		if (is_validataion_needed)
		{
			setup_debug_utils_messenger ();
		}

		get_physical_device ();
		create_surface (hInstance, hWnd);
		populate_graphics_device_extensions ();
		create_graphics_device_and_queue ();
		create_swapchain ();
		create_command_pool ();
		create_sampler ();
	}

	void exit ()
	{
		OutputDebugString (L"common_graphics::exit\n");

		if (common_graphics_obj_ptr->common_sampler != VK_NULL_HANDLE)
		{
			common_graphics_obj_ptr->graphics_device.destroySampler (common_graphics_obj_ptr->common_sampler);
		}

		if (common_graphics_obj_ptr->command_pool != VK_NULL_HANDLE)
		{
			common_graphics_obj_ptr->graphics_device.destroyCommandPool (common_graphics_obj_ptr->command_pool);
		}

		for (auto& swapchain_imageview : common_graphics_obj_ptr->swapchain_imageviews)
		{
			if (swapchain_imageview != VK_NULL_HANDLE)
			{
				common_graphics_obj_ptr->graphics_device.destroyImageView (swapchain_imageview);
			}
		}

		if (common_graphics_obj_ptr->swapchain != VK_NULL_HANDLE)
		{
			common_graphics_obj_ptr->graphics_device.destroySwapchainKHR (common_graphics_obj_ptr->swapchain);
		}

		if (common_graphics_obj_ptr->graphics_device != VK_NULL_HANDLE)
		{
			common_graphics_obj_ptr->graphics_device.destroy ();
		}

		if (surface != VK_NULL_HANDLE)
		{
			instance.destroySurfaceKHR (surface);
		}

		instance.destroyDebugUtilsMessengerEXT (debug_utils_messenger);

		if (instance != VK_NULL_HANDLE)
		{
			instance.destroy ();
		}
	}
}