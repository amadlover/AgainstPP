#include <Windows.h>

#include "graphics_utils.hpp"
#include "common_graphics.hpp"
#include "error.hpp"

#include <vulkan/vulkan_win32.h>


VkInstance common_graphics::instance;
VkPhysicalDevice common_graphics::physical_device;
VkDevice common_graphics::graphics_device;
uint32_t common_graphics::graphics_queue_family_index;
VkQueue common_graphics::graphics_queue;
VkPhysicalDeviceMemoryProperties common_graphics::physical_device_memory_properties;
VkPhysicalDeviceLimits common_graphics::physical_device_limits;
VkSurfaceFormatKHR common_graphics::chosen_surface_format;
VkExtent2D common_graphics::surface_extent;
VkSwapchainKHR common_graphics::swapchain;
uint32_t common_graphics::swapchain_image_count;
std::vector<VkImage> common_graphics::swapchain_images;
std::vector<VkImageView> common_graphics::swapchain_imageviews;
VkSampler common_graphics::common_sampler;

std::vector<const char*> requested_instance_layers;
std::vector<const char*> requested_instance_extensions;
std::vector<const char*> requested_device_extensions;

bool is_validation_needed;

VkDebugUtilsMessengerEXT debug_utils_messenger;
VkSurfaceKHR surface;
VkPresentModeKHR chosen_present_mode;

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

egraphics_result populate_instance_layers_and_extensions ()
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

	return egraphics_result::success;
}

egraphics_result create_instance ()
{
	VkApplicationInfo application_info = {};

	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pEngineName = "AGE";
	application_info.apiVersion = VK_API_VERSION_1_2;
	application_info.applicationVersion = VK_MAKE_VERSION (1, 0, 0);
	application_info.engineVersion = VK_MAKE_VERSION (1, 0, 0);
	application_info.pApplicationName = "Against";

	VkInstanceCreateInfo create_info = {};

	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.enabledExtensionCount = requested_instance_extensions.size ();
	create_info.ppEnabledExtensionNames = requested_instance_extensions.data ();
	create_info.enabledLayerCount = requested_instance_layers.size ();
	create_info.ppEnabledLayerNames = requested_instance_layers.data ();
	create_info.pApplicationInfo = &application_info;
	create_info.flags = 0;

	if (vkCreateInstance (&create_info, nullptr, &common_graphics::instance) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_instance;
	}

	return egraphics_result::success;
}

egraphics_result setup_debug_utils_messenger ()
{
	VkDebugUtilsMessengerCreateInfoEXT create_info = {};

	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	create_info.pfnUserCallback = debug_messenger_callback;
	create_info.flags = 0;

	if (create_debug_utils_messenger (common_graphics::instance, &create_info, nullptr, &debug_utils_messenger) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_setup_debug_utils_messenger;
	}

	return egraphics_result::success;
}

egraphics_result get_physical_device ()
{
	uint32_t physical_device_count = 0;
	vkEnumeratePhysicalDevices (common_graphics::instance, &physical_device_count, nullptr);
	std::vector<VkPhysicalDevice> physical_devices (physical_device_count);
	vkEnumeratePhysicalDevices (common_graphics::instance, &physical_device_count, physical_devices.data ());

	if (physical_device_count == 0)
	{
		return egraphics_result::e_against_error_graphics_get_physical_device;
	}

	common_graphics::physical_device = physical_devices[0];

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures (common_graphics::physical_device, &device_features);

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties (common_graphics::physical_device, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_family_properties (queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties (common_graphics::physical_device, &queue_family_count, queue_family_properties.data ());

	for (uint32_t i = 0; i < queue_family_count; i++)
	{
		if ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (queue_family_properties[i].queueCount > 1))
		{
			common_graphics::graphics_queue_family_index = i;
			break;
		}
	}

	vkGetPhysicalDeviceMemoryProperties (common_graphics::physical_device, &common_graphics::physical_device_memory_properties);

	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties (common_graphics::physical_device, &device_properties);
	common_graphics::physical_device_limits = device_properties.limits;

	return egraphics_result::success;
}

egraphics_result create_surface (HINSTANCE HInstance, HWND HWnd)
{
	VkWin32SurfaceCreateInfoKHR create_info = {};

	create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	create_info.hinstance = HInstance;
	create_info.hwnd = HWnd;

	if (vkCreateWin32SurfaceKHR (common_graphics::instance, &create_info, nullptr, &surface) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_surface;
	}

	return egraphics_result::success;
}

egraphics_result populate_graphics_device_extensions ()
{
	uint32_t extension_count = 0;
	vkEnumerateDeviceExtensionProperties (common_graphics::physical_device, nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extension_properties (extension_count);
	vkEnumerateDeviceExtensionProperties (common_graphics::physical_device, nullptr, &extension_count, extension_properties.data ());

	for (uint32_t e = 0; e < extension_count; e++)
	{
		if (strcmp (extension_properties[e].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			requested_device_extensions.push_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			break;
		}
	}

	return egraphics_result::success;
}

egraphics_result create_graphics_device ()
{
	float priorities = 1.f;

	VkDeviceQueueCreateInfo queue_create_info = {};

	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.pNext = NULL;
	queue_create_info.pQueuePriorities = &priorities;
	queue_create_info.queueCount = 1;
	queue_create_info.queueFamilyIndex = common_graphics::graphics_queue_family_index;
	queue_create_info.flags = 0;

	VkDeviceCreateInfo create_info = {};

	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pNext = NULL;
	create_info.enabledExtensionCount = requested_device_extensions.size ();
	create_info.ppEnabledExtensionNames = requested_device_extensions.data ();
	create_info.enabledLayerCount = 0;
	create_info.ppEnabledLayerNames = nullptr;
	create_info.queueCreateInfoCount = 1;
	create_info.pQueueCreateInfos = &queue_create_info;
	create_info.flags = 0;

	if (vkCreateDevice (common_graphics::physical_device, &create_info, nullptr, &common_graphics::graphics_device) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_graphics_device;
	}

	vkGetDeviceQueue (common_graphics::graphics_device, common_graphics::graphics_queue_family_index, 0, &common_graphics::graphics_queue);

	return egraphics_result::success;
}

egraphics_result create_swapchain ()
{
	VkBool32 is_surface_supported = false;
	vkGetPhysicalDeviceSurfaceSupportKHR (common_graphics::physical_device, common_graphics::graphics_queue_family_index, surface, &is_surface_supported);

	if (!is_surface_supported)
	{
		return egraphics_result::e_against_error_graphics_surface_support;
	}

	VkSurfaceCapabilitiesKHR surface_capabilites;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR (common_graphics::physical_device, surface, &surface_capabilites);

	uint32_t surface_format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR (common_graphics::physical_device, surface, &surface_format_count, nullptr);

	std::vector<VkSurfaceFormatKHR> surface_formats (surface_format_count);
	vkGetPhysicalDeviceSurfaceFormatsKHR (common_graphics::physical_device, surface, &surface_format_count, surface_formats.data ());

	for (const auto& surface_format : surface_formats)
	{
		if (surface_format.format == VK_FORMAT_B8G8R8A8_UNORM)
		{
			common_graphics::chosen_surface_format = surface_format;
			break;
		}
	}

	uint32_t present_mode_count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR (common_graphics::physical_device, surface, &present_mode_count, nullptr);

	std::vector<VkPresentModeKHR> present_modes (present_mode_count);
	vkGetPhysicalDeviceSurfacePresentModesKHR (common_graphics::physical_device, surface, &present_mode_count, present_modes.data ());

	for (const auto& present_mode : present_modes)
	{
		if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			chosen_present_mode = present_mode;
			break;
		}
	}

	common_graphics::surface_extent = surface_capabilites.currentExtent;

	VkSwapchainCreateInfoKHR create_info = {};

	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;

	create_info.clipped = 1;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.imageArrayLayers = 1;
	create_info.imageColorSpace = common_graphics::chosen_surface_format.colorSpace;
	create_info.imageExtent = surface_capabilites.currentExtent;
	create_info.imageFormat = common_graphics::chosen_surface_format.format;
	create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	create_info.imageUsage = surface_capabilites.supportedUsageFlags;
	create_info.minImageCount = surface_capabilites.minImageCount + 1;
	create_info.oldSwapchain = VK_NULL_HANDLE;
	create_info.presentMode = chosen_present_mode;
	create_info.preTransform = surface_capabilites.currentTransform;

	if (vkCreateSwapchainKHR (common_graphics::graphics_device, &create_info, nullptr, &common_graphics::swapchain) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_swapchain;
	}

	vkGetSwapchainImagesKHR (common_graphics::graphics_device, common_graphics::swapchain, &common_graphics::swapchain_image_count, nullptr);
	common_graphics::swapchain_images.resize (common_graphics::swapchain_image_count);
	vkGetSwapchainImagesKHR (common_graphics::graphics_device, common_graphics::swapchain, &common_graphics::swapchain_image_count, common_graphics::swapchain_images.data ());
	
	common_graphics::swapchain_imageviews.resize (common_graphics::swapchain_image_count);

	return egraphics_result::success;
}

egraphics_result create_swapchain_imageviews ()
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

	VkImageViewCreateInfo create_info = {};

	create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	create_info.format = common_graphics::chosen_surface_format.format;
	create_info.components = components;
	create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	create_info.subresourceRange = subresource_range;

	for (uint32_t i = 0; i < common_graphics::swapchain_image_count; i++)
	{
		create_info.image = common_graphics::swapchain_images[i];
		if (vkCreateImageView (common_graphics::graphics_device, &create_info, nullptr, &common_graphics::swapchain_imageviews[i]) != VK_SUCCESS)
		{
			return egraphics_result::e_against_error_graphics_create_image_view;
		}
	}

	return egraphics_result::success;
}

egraphics_result create_sampler ()
{
	VkSamplerCreateInfo create_info = {};
	
	create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	create_info.minFilter = VK_FILTER_LINEAR;
	create_info.magFilter = VK_FILTER_LINEAR;
	create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

	if (vkCreateSampler (common_graphics::graphics_device, &create_info, nullptr, &common_graphics::common_sampler) != VK_SUCCESS)
	{
		return egraphics_result::e_against_error_graphics_create_sampler;
	}

	return egraphics_result::success;
}

egraphics_result common_graphics::init (HINSTANCE hInstance, HWND hWnd)
{
	#ifdef DEBUG
	is_validation_needed = true;
#elif _DEBUG
	is_validation_needed = true;
#else
	is_validation_needed = false;
#endif

	CHECK_AGAINST_RESULT (populate_instance_layers_and_extensions ());
	CHECK_AGAINST_RESULT (create_instance ());
	if (is_validation_needed)
	{
		CHECK_AGAINST_RESULT (setup_debug_utils_messenger ());
	}

	CHECK_AGAINST_RESULT (get_physical_device ());
	CHECK_AGAINST_RESULT (create_surface (hInstance, hWnd));
	CHECK_AGAINST_RESULT (populate_graphics_device_extensions ());
	CHECK_AGAINST_RESULT (create_graphics_device ());
	CHECK_AGAINST_RESULT (create_swapchain ());
	CHECK_AGAINST_RESULT (create_swapchain_imageviews ());
	CHECK_AGAINST_RESULT (create_sampler ());

	swapchain_images.shrink_to_fit ();
	swapchain_imageviews.shrink_to_fit ();

	return egraphics_result::success;
}

void common_graphics::exit ()
{


	vkDestroySampler (graphics_device, common_sampler, nullptr);

	vkDestroySwapchainKHR (graphics_device, swapchain, nullptr);

	for (auto& swapchain_imageview : swapchain_imageviews)
	{
		vkDestroyImageView (graphics_device, swapchain_imageview, nullptr);
	}
	swapchain_imageviews.clear ();

	vkDestroyDevice (graphics_device, nullptr);
	vkDestroySurfaceKHR (instance, surface, nullptr);
	
	if (is_validation_needed)
	{
		destroy_debug_utils_messenger (instance, debug_utils_messenger, nullptr);
	}
	vkDestroyInstance (instance, nullptr);
}
