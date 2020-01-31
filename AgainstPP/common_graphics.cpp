#include <Windows.h>

#include "graphics_utils.hpp"
#include "common_graphics.hpp"
#include "Error.hpp"

#include <vulkan/vulkan_win32.h>

/*PFN_vkCreateDebugUtilsMessengerEXT pfn_vk_create_debug_utils_messenger_ext;
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
		/*

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
			&common_graphics_obj_ptr->graphics_queue_family_indices[0],
			surface_capabilities.currentTransform,
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			chosen_present_mode,
			VK_TRUE
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
}*/

VkResult create_debug_utils_messenger (VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* debug_utils_messenger_create_info,
	const VkAllocationCallbacks* allocation_callbacks,
	VkDebugUtilsMessengerEXT* debug_utils_messenger)
{
	PFN_vkCreateDebugUtilsMessengerEXT Func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr (instance, "vkCreateDebugUtilsMessengerEXT");

	if (Func)
	{
		return Func (instance, debug_utils_messenger_create_info, allocation_callbacks, debug_utils_messenger);
	}
	else
	{
		return VK_ERROR_INITIALIZATION_FAILED;
	}
}

void destroy_debug_utils_messenger (VkInstance instance,
	VkDebugUtilsMessengerEXT debug_utils_messenger,
	const VkAllocationCallbacks* allocation_callbacks)
{
	PFN_vkDestroyDebugUtilsMessengerEXT Func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr (instance, "vkDestroyDebugUtilsMessengerEXT");

	if (Func)
	{
		Func (instance, debug_utils_messenger, allocation_callbacks);
	}
	else
	{
		OutputDebugString (L"Could not Destroy Debug Utils Messenger\n");
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_messenger_callback (
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_types,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void* pUserData)
{
	if (callback_data)
	{
		wchar_t buff[2048];
		swprintf_s (buff, 2048, L"Debug Messenger Callback: %hs\n", callback_data->pMessage);
		OutputDebugString (buff);
	}

	return 0;
}

common_graphics::common_graphics ()
{
	OutputDebugString (L"common_graphics::common_graphics\n");
}

void common_graphics::populate_instance_layers_and_extensions ()
{
	if (is_validation_needed)
	{
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties (&layer_count, nullptr);
		std::vector<VkLayerProperties> layer_properties (layer_count);
		vkEnumerateInstanceLayerProperties (&layer_count, layer_properties.data ());

		for (uint32_t l = 0; l < layer_count; l++)
		{
			if (strcmp (layer_properties[l].layerName, "VK_LAYER_LUNARG_standard_validation") == 0)
			{
				requested_instance_layers.push_back ("VK_LAYER_LUNARG_standard_validation");
				break;
			}
		}
	}

	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties (nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> extension_properties (extension_count);
	vkEnumerateInstanceExtensionProperties (nullptr, &extension_count, extension_properties.data ());

	for (uint32_t e = 0; e < extension_count; e++)
	{
		if (strcmp (extension_properties[e].extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0)
		{
			requested_instance_extensions.push_back (VK_KHR_SURFACE_EXTENSION_NAME);
		}
		else if (strcmp (extension_properties[e].extensionName, "VK_KHR_win32_surface") == 0)
		{
			requested_instance_extensions.push_back ("VK_KHR_win32_surface");
		}

		if (is_validation_needed)
		{
			if (strcmp (extension_properties[e].extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
			{
				requested_instance_extensions.push_back (VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}
		}
	}
}

void common_graphics::create_instance ()
{
	VkApplicationInfo application_info = { 0 };

	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pEngineName = "AGE";
	application_info.apiVersion = VK_API_VERSION_1_2;
	application_info.applicationVersion = VK_MAKE_VERSION (1, 0, 0);
	application_info.engineVersion = VK_MAKE_VERSION (1, 0, 0);
	application_info.pApplicationName = "Against";

	VkInstanceCreateInfo create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.enabledExtensionCount = requested_instance_extensions.size ();
	create_info.ppEnabledExtensionNames = requested_instance_extensions.data ();
	create_info.enabledLayerCount = requested_instance_layers.size ();
	create_info.ppEnabledLayerNames = requested_instance_layers.data ();
	create_info.pApplicationInfo = &application_info;
	create_info.flags = 0;

	if (vkCreateInstance (&create_info, nullptr, &instance) != VK_SUCCESS)
	{
		throw egraphics_error::create_instance;
	}
}

void common_graphics::setup_debug_utils_messenger ()
{
	VkDebugUtilsMessengerCreateInfoEXT create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	create_info.pfnUserCallback = debug_messenger_callback;
	create_info.flags = 0;

	if (create_debug_utils_messenger (instance, &create_info, nullptr, &debug_utils_messenger) != VK_SUCCESS)
	{
		throw egraphics_error::setup_debug_utils_messenger;
	}
}

void common_graphics::get_physical_device ()
{
	uint32_t physical_device_count = 0;
	vkEnumeratePhysicalDevices (instance, &physical_device_count, nullptr);
	std::vector<VkPhysicalDevice> physical_devices (physical_device_count);
	vkEnumeratePhysicalDevices (instance, &physical_device_count, physical_devices.data ());

	if (physical_device_count == 0)
	{
		throw egraphics_error::get_physical_device;
	}

	physical_device = physical_devices[0];

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures (physical_device, &device_features);

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties (physical_device, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_family_properties (queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties (physical_device, &queue_family_count, queue_family_properties.data ());

	for (uint32_t i = 0; i < queue_family_count; i++)
	{
		if ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (queue_family_properties[i].queueCount > 1))
		{
			graphics_queue_family_index = i;
			break;
		}
	}

	vkGetPhysicalDeviceMemoryProperties (physical_device, &physical_device_memory_properties);

	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties (physical_device, &device_properties);
	physical_device_limits = device_properties.limits;
}

void common_graphics::create_surface (HINSTANCE HInstance, HWND HWnd)
{
	VkWin32SurfaceCreateInfoKHR create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	create_info.hinstance = HInstance;
	create_info.hwnd = HWnd;

	if (vkCreateWin32SurfaceKHR (instance, &create_info, nullptr, &surface) != VK_SUCCESS)
	{
		throw egraphics_error::create_surface;
	}
}

void common_graphics::populate_graphics_device_extensions ()
{
	uint32_t extension_count = 0;
	vkEnumerateDeviceExtensionProperties (physical_device, nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extension_properties (extension_count);
	vkEnumerateDeviceExtensionProperties (physical_device, nullptr, &extension_count, extension_properties.data ());

	for (uint32_t e = 0; e < extension_count; e++)
	{
		if (strcmp (extension_properties[e].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			requested_device_extensions.push_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			break;
		}
	}
}

void common_graphics::create_graphics_device ()
{
	float priorities = 1.f;

	VkDeviceQueueCreateInfo queue_create_info = { 0 };

	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.pNext = NULL;
	queue_create_info.pQueuePriorities = &priorities;
	queue_create_info.queueCount = 1;
	queue_create_info.queueFamilyIndex = graphics_queue_family_index;
	queue_create_info.flags = 0;

	VkDeviceCreateInfo create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pNext = NULL;
	create_info.enabledExtensionCount = requested_device_extensions.size ();
	create_info.ppEnabledExtensionNames = requested_device_extensions.data ();
	create_info.enabledLayerCount = 0;
	create_info.ppEnabledLayerNames = nullptr;
	create_info.queueCreateInfoCount = 1;
	create_info.pQueueCreateInfos = &queue_create_info;
	create_info.flags = 0;

	if (vkCreateDevice (physical_device, &create_info, nullptr, &graphics_device) != VK_SUCCESS)
	{
		throw egraphics_error::create_graphics_device;
	}

	vkGetDeviceQueue (graphics_device, graphics_queue_family_index, 0, &graphics_queue);
}

void common_graphics::create_swapchain ()
{
	VkBool32 is_surface_supported = false;
	vkGetPhysicalDeviceSurfaceSupportKHR (physical_device, graphics_queue_family_index, surface, &is_surface_supported);

	if (!is_surface_supported)
	{
		throw egraphics_error::surface_support;
	}

	VkSurfaceCapabilitiesKHR surface_capabilites;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR (physical_device, surface, &surface_capabilites);

	uint32_t surface_format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR (physical_device, surface, &surface_format_count, nullptr);

	std::vector<VkSurfaceFormatKHR> surface_formats (surface_format_count);
	vkGetPhysicalDeviceSurfaceFormatsKHR (physical_device, surface, &surface_format_count, surface_formats.data ());

	for (const auto& surface_format : surface_formats)
	{
		if (surface_format.format == VK_FORMAT_B8G8R8A8_UNORM)
		{
			chosen_surface_format = surface_format;
			break;
		}
	}

	uint32_t present_mode_count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR (physical_device, surface, &present_mode_count, nullptr);

	std::vector<VkPresentModeKHR> present_modes (present_mode_count);
	vkGetPhysicalDeviceSurfacePresentModesKHR (physical_device, surface, &present_mode_count, present_modes.data ());

	for (const auto& present_mode : present_modes)
	{
		if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			chosen_present_mode = present_mode;
			break;
		}
	}

	surface_extent = surface_capabilites.currentExtent;

	VkSwapchainCreateInfoKHR create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;

	create_info.clipped = 1;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.imageArrayLayers = 1;
	create_info.imageColorSpace = chosen_surface_format.colorSpace;
	create_info.imageExtent = surface_capabilites.currentExtent;
	create_info.imageFormat = chosen_surface_format.format;
	create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	create_info.imageUsage = surface_capabilites.supportedUsageFlags;
	create_info.minImageCount = surface_capabilites.minImageCount + 1;
	create_info.oldSwapchain = VK_NULL_HANDLE;
	create_info.presentMode = chosen_present_mode;
	create_info.preTransform = surface_capabilites.currentTransform;

	if (vkCreateSwapchainKHR (graphics_device, &create_info, nullptr, &swapchain) != VK_SUCCESS)
	{
		throw egraphics_error::create_swapchain;
	}

	vkGetSwapchainImagesKHR (graphics_device, swapchain, &swapchain_image_count, nullptr);
	swapchain_images.resize (swapchain_image_count);
	vkGetSwapchainImagesKHR (graphics_device, swapchain, &swapchain_image_count, swapchain_images.data ());
	
	swapchain_imageviews.resize (swapchain_image_count);
}

void common_graphics::create_swapchain_imageviews ()
{
	VkComponentMapping components;

	components.a = VK_COMPONENT_SWIZZLE_A;
	components.b = VK_COMPONENT_SWIZZLE_B;
	components.g = VK_COMPONENT_SWIZZLE_G;
	components.r = VK_COMPONENT_SWIZZLE_R;

	VkImageSubresourceRange subresource_range;

	subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource_range.baseMipLevel = 0;
	subresource_range.levelCount = 1;

	subresource_range.baseArrayLayer = 0;
	subresource_range.layerCount = 1;

	VkImageViewCreateInfo create_info = { 0 };

	create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	create_info.format = chosen_surface_format.format;
	create_info.components = components;
	create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	create_info.subresourceRange = subresource_range;

	for (uint32_t i = 0; i < swapchain_image_count; i++)
	{
		create_info.image = swapchain_images[i];
		if (vkCreateImageView (graphics_device, &create_info, nullptr, &swapchain_imageviews[i]) != VK_SUCCESS)
		{
			throw egraphics_error::create_image_view;
		}
	}
}

void common_graphics::create_command_pool ()
{
	VkCommandPoolCreateInfo command_pool_create_info = { 0 };
	command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create_info.queueFamilyIndex = graphics_queue_family_index;
	command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool (graphics_device, &command_pool_create_info, nullptr, &command_pool) != VK_SUCCESS)
	{
		throw egraphics_error::create_command_pool;
	}
}

void common_graphics::create_sampler ()
{
	VkSamplerCreateInfo create_info = { 0 };
	
	create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	create_info.minFilter = VK_FILTER_LINEAR;
	create_info.magFilter = VK_FILTER_LINEAR;
	create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

	if (vkCreateSampler (graphics_device, &create_info, nullptr, &common_sampler) != VK_SUCCESS)
	{
		throw egraphics_error::create_sampler;
	}
}

common_graphics::~common_graphics ()
{
	OutputDebugString (L"common_graphics::~common_graphics\n");
}

void common_graphics::init (HINSTANCE hInstance, HWND hWnd)
{
	#ifdef DEBUG
	is_validation_needed = true;
#elif _DEBUG
	is_validation_needed = true;
#else
	is_validation_needed = false;
#endif

	populate_instance_layers_and_extensions ();
	create_instance ();
	if (is_validation_needed)
	{
		setup_debug_utils_messenger ();
	}

	get_physical_device ();
	create_surface (hInstance, hWnd);
	populate_graphics_device_extensions ();
	create_graphics_device ();
	create_swapchain ();
	create_swapchain_imageviews ();
	create_command_pool ();
	create_sampler ();
}

void common_graphics::exit ()
{

	if (common_sampler != VK_NULL_HANDLE)
	{
		vkDestroySampler (graphics_device, common_sampler, nullptr);
	}

	if (command_pool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool (graphics_device, command_pool, nullptr);
	}

	if (swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR (graphics_device, swapchain, nullptr);
	}

	for (auto& swapchain_imageview : swapchain_imageviews)
	{
		if (swapchain_imageview != VK_NULL_HANDLE)
		{
			vkDestroyImageView (graphics_device, swapchain_imageview, nullptr);
		}
	}

	if (graphics_device != VK_NULL_HANDLE)
	{
		vkDestroyDevice (graphics_device, nullptr);
	}

	if (surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR (instance, surface, nullptr);
	}

	if (is_validation_needed)
	{
		if (debug_utils_messenger != VK_NULL_HANDLE)
		{
			destroy_debug_utils_messenger (instance, debug_utils_messenger, nullptr);
		}
	}

	if (instance != VK_NULL_HANDLE)
	{
		vkDestroyInstance (instance, nullptr);
	}	
}
