#include <Windows.h>

#include "Graphics.hpp"
#include "Error.hpp"

#include <vulkan/vulkan_win32.h>

VkResult CreateDebugUtilsMessenger (VkInstance Instance, const VkDebugUtilsMessengerCreateInfoEXT* DebugUtilsMessengerCreateInfo, const VkAllocationCallbacks* AllocationCallbacks, VkDebugUtilsMessengerEXT* DebugUtilsMessenger)
{
	OutputDebugString (L"CreateDebugUtilsMessenger\n");

	PFN_vkCreateDebugUtilsMessengerEXT Func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr (Instance, "vkCreateDebugUtilsMessengerEXT");

	if (Func)
	{
		return Func (Instance, DebugUtilsMessengerCreateInfo, AllocationCallbacks, DebugUtilsMessenger);
	}
	else
	{
		return VK_ERROR_INITIALIZATION_FAILED;
	}
}

void DestroyDebugUtilsMessenger (VkInstance Instance, VkDebugUtilsMessengerEXT DebugUtilsMessenger, const VkAllocationCallbacks* AllocationCallbacks)
{
	OutputDebugString (L"DestroyDebugUtilsMessenger\n");

	PFN_vkDestroyDebugUtilsMessengerEXT Func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr (Instance, "vkDestroyDebugUtilsMessengerEXT");

	if (Func)
	{
		Func (Instance, DebugUtilsMessenger, AllocationCallbacks);
	}
	else
	{
		OutputDebugString (L"Could not Destroy Debug Utils Messenger\n");
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback (VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (pCallbackData)
	{
		wchar_t buff[2048];
		swprintf_s (buff, 2048, L"Debug Messenger Callback: %hs\n", pCallbackData->pMessage);
		OutputDebugString (buff);
	}

	return VK_FALSE;
}

Graphics::Graphics (HINSTANCE HInstance, HWND HWnd)
{
	OutputDebugString (L"Graphics::Graphics\n");

#ifdef _DEBUG
	_IsValidationNeeded = true;
#else
	_IsValidationNeeded = false;
#endif

	_PopulateInstanceLayersAndExtensions ();
	_CreateInstance ();

	if (_IsValidationNeeded)
	{
		_SetupDebugUtilsMessenger ();
	}

	_GetPhysicalDevice ();
	_CreateSurface (HInstance, HWnd);
	_PopulateGraphicsDeviceExtensions ();
	_CreateGraphicsDevice ();
}

void Graphics::_PopulateInstanceLayersAndExtensions ()
{
	OutputDebugString (L"Graphics::_PopulateInstanceLayersAndExtensions\n");

	if (_IsValidationNeeded)
	{
		uint32_t LayerCount = 0;
		vkEnumerateInstanceLayerProperties (&LayerCount, NULL);
		std::vector<VkLayerProperties> LayerProperties (LayerCount);
		vkEnumerateInstanceLayerProperties (&LayerCount, LayerProperties.data ());

		for (auto LayerProperty : LayerProperties)
		{
			if (strcmp (LayerProperty.layerName, "VK_LAYER_LUNARG_standard_validation") == 0)
			{
				RequestedInstanceLayers[RequestedInstanceLayerCount++] = ("VK_LAYER_LUNARG_standard_validation");
				break;
			}
		}
	}

	uint32_t ExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties (NULL, &ExtensionCount, NULL);

	std::vector<VkExtensionProperties> ExtensionProperties (ExtensionCount);
	vkEnumerateInstanceExtensionProperties (NULL, &ExtensionCount, ExtensionProperties.data ());

	for (auto ExtensionProperty : ExtensionProperties)
	{
		if (strcmp (ExtensionProperty.extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0)
		{
			RequestedInstanceExtensions[RequestedInstanceExtensionCount++] = VK_KHR_SURFACE_EXTENSION_NAME;
		}
		else if (strcmp (ExtensionProperty.extensionName, "VK_KHR_win32_surface") == 0)
		{
			RequestedInstanceExtensions[RequestedInstanceExtensionCount++] = "VK_KHR_win32_surface";
		}
		else if (strcmp (ExtensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			RequestedInstanceExtensions[RequestedInstanceExtensionCount++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
		}

		if (_IsValidationNeeded)
		{
			if (strcmp (ExtensionProperty.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
			{
				RequestedInstanceExtensions[RequestedInstanceExtensionCount++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
			}
		}
	}
}

void Graphics::_CreateInstance ()
{
	OutputDebugString (L"Graphics::_CreateInstance\n");

	VkApplicationInfo ApplicationInfo;
	memset (&ApplicationInfo, 0, sizeof (VkApplicationInfo));

	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.pEngineName = "AGE";
	ApplicationInfo.apiVersion = VK_API_VERSION_1_1;
	ApplicationInfo.applicationVersion = VK_MAKE_VERSION (1, 0, 0);
	ApplicationInfo.engineVersion = VK_MAKE_VERSION (1, 0, 0);
	ApplicationInfo.pApplicationName = "Against";

	VkInstanceCreateInfo CreateInfo;
	memset (&CreateInfo, 0, sizeof (VkInstanceCreateInfo));

	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateInfo.enabledExtensionCount = RequestedInstanceExtensionCount;
	CreateInfo.ppEnabledExtensionNames = RequestedInstanceExtensions;
	CreateInfo.enabledLayerCount = RequestedInstanceLayerCount;
	CreateInfo.ppEnabledLayerNames = RequestedInstanceLayers;
	CreateInfo.pApplicationInfo = &ApplicationInfo;
	CreateInfo.flags = 0;

	if (vkCreateInstance (&CreateInfo, NULL, &Instance) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_INSTANCE;
	}
}

void Graphics::_SetupDebugUtilsMessenger ()
{
	OutputDebugString (L"Graphics::_SetupDebugUtilsMessenger\n");

	VkDebugUtilsMessengerCreateInfoEXT CreateInfo;
	memset (&CreateInfo, 0, sizeof (VkDebugUtilsMessengerCreateInfoEXT));

	CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	CreateInfo.messageSeverity = /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |*/ VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	CreateInfo.pfnUserCallback = DebugMessengerCallback;
	CreateInfo.flags = 0;

	if (CreateDebugUtilsMessenger (Instance, &CreateInfo, NULL, &DebugUtilsMessenger) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_DEBUG_UTILS_MESSENGER;
	}
}

void Graphics::_GetPhysicalDevice ()
{
	OutputDebugString (L"Graphics::_GetPhysicalDevice\n");

	uint32_t PhysicalDeviceCount = 0;
	vkEnumeratePhysicalDevices (Instance, &PhysicalDeviceCount, NULL);

	std::vector<VkPhysicalDevice> PhysicalDevices (PhysicalDeviceCount);
	vkEnumeratePhysicalDevices (Instance, &PhysicalDeviceCount, PhysicalDevices.data ());

	if (PhysicalDeviceCount == 0)
	{
		throw GraphicsError::eGET_PHYSICAL_DEVICE;
	}

	PhysicalDevice = PhysicalDevices[0];

	VkPhysicalDeviceFeatures DeviceFeatures;
	vkGetPhysicalDeviceFeatures (PhysicalDevice, &DeviceFeatures);

	uint32_t QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties (PhysicalDevice, &QueueFamilyCount, NULL);
	std::vector<VkQueueFamilyProperties> QueueFamilyProperties (QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties (PhysicalDevice, &QueueFamilyCount, QueueFamilyProperties.data ());

	for (uint32_t i = 0; i < QueueFamilyCount; i++)
	{
		if ((QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (QueueFamilyProperties[i].queueCount > 1))
		{
			GraphicsQueueFamilyIndex = i;
			break;
		}
	}

	vkGetPhysicalDeviceMemoryProperties (PhysicalDevice, &PhysicalDeviceMemoryProperties);

	VkPhysicalDeviceProperties DeviceProperties;
	vkGetPhysicalDeviceProperties (PhysicalDevice, &DeviceProperties);
	PhysicalDeviceLimits = DeviceProperties.limits;
}

void Graphics::_CreateSurface (HINSTANCE HInstance, HWND HWnd)
{
	OutputDebugString (L"Graphics::_CreateSurface\n");

	VkWin32SurfaceCreateInfoKHR CreateInfo;
	memset (&CreateInfo, 0, sizeof (VkWin32SurfaceCreateInfoKHR));

	CreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	CreateInfo.hinstance = HInstance;
	CreateInfo.hwnd = HWnd;

	if (vkCreateWin32SurfaceKHR (Instance, &CreateInfo, NULL, &Surface) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_SURFACE;
	}
}

void Graphics::_PopulateGraphicsDeviceExtensions ()
{
	OutputDebugString (L"Graphics::_PopulateGraphicsDeviceExtensions\n");

	uint32_t ExtensionCount = 0;
	vkEnumerateDeviceExtensionProperties (PhysicalDevice, NULL, &ExtensionCount, NULL);

	std::vector<VkExtensionProperties> ExtensionProperties (ExtensionCount);
	vkEnumerateDeviceExtensionProperties (PhysicalDevice, NULL, &ExtensionCount, ExtensionProperties.data ());

	for (auto ExtensionProperty : ExtensionProperties)
	{
		if (strcmp (ExtensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			RequestedDeviceExtensions[RequestedDeviceExtensionCount++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
			break;
		}
	}
}

void Graphics::_CreateGraphicsDevice ()
{
	OutputDebugString (L"Graphics::_CreateGraphicsDevice\n");

	float Priorities = 1.f;

	VkDeviceQueueCreateInfo QueueCreateInfo;
	memset (&QueueCreateInfo, 0, sizeof (VkDeviceQueueCreateInfo));

	QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	QueueCreateInfo.pNext = NULL;
	QueueCreateInfo.pQueuePriorities = &Priorities;
	QueueCreateInfo.queueCount = 1;
	QueueCreateInfo.queueFamilyIndex = GraphicsQueueFamilyIndex;
	QueueCreateInfo.flags = 0;

	VkDeviceCreateInfo CreateInfo;
	memset (&CreateInfo, 0, sizeof (VkDeviceCreateInfo));

	CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	CreateInfo.pNext = NULL;
	CreateInfo.enabledExtensionCount = RequestedDeviceExtensionCount;
	CreateInfo.ppEnabledExtensionNames = RequestedDeviceExtensions;
	CreateInfo.enabledLayerCount = 0;
	CreateInfo.ppEnabledLayerNames = NULL;
	CreateInfo.queueCreateInfoCount = 1;
	CreateInfo.pQueueCreateInfos = &QueueCreateInfo;
	CreateInfo.flags = 0;

	if (vkCreateDevice (PhysicalDevice, &CreateInfo, NULL, &GraphicsDevice) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_GRAPHICS_DEVICE;
	}

	vkGetDeviceQueue (GraphicsDevice, GraphicsQueueFamilyIndex, 0, &GraphicsQueue);
}

void Graphics::_CreateSwapChain ()
{
	OutputDebugString (L"CreateSwapChain\n");

	VkBool32 IsSurfaceSupported = false;
	vkGetPhysicalDeviceSurfaceSupportKHR (PhysicalDevice, GraphicsQueueFamilyIndex, Surface, &IsSurfaceSupported);

	if (!IsSurfaceSupported)
	{
		throw GraphicsError::eSURFACE_SUPPORT;
	}

	VkSurfaceCapabilitiesKHR SurfaceCapabilites;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR (PhysicalDevice, Surface, &SurfaceCapabilites);

	uint32_t SurfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR (PhysicalDevice, Surface, &SurfaceFormatCount, NULL);

	std::vector<VkSurfaceFormatKHR> SurfaceFormats (SurfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR (PhysicalDevice, Surface, &SurfaceFormatCount, SurfaceFormats.data ());

	for (auto SurfaceFormat : SurfaceFormats)
	{
		if (SurfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
		{
			ChosenSurfaceFormat = SurfaceFormat;
			break;
		}
	}

	uint32_t PresentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR (PhysicalDevice, Surface, &PresentModeCount, NULL);

	std::vector<VkPresentModeKHR> PresentModes (PresentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR (PhysicalDevice, Surface, &PresentModeCount, PresentModes.data ());

	for (auto PresentMode : PresentModes)
	{
		if (PresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			ChosenPresentMode = PresentMode;
			break;
		}
	}

	SurfaceExtent = SurfaceCapabilites.currentExtent;

	VkSwapchainCreateInfoKHR CreateInfo;
	memset (&CreateInfo, 0, sizeof (VkSwapchainCreateInfoKHR));

	CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	CreateInfo.surface = Surface;

	CreateInfo.clipped = VK_TRUE;
	CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	CreateInfo.imageArrayLayers = 1;
	CreateInfo.imageColorSpace = ChosenSurfaceFormat.colorSpace;
	CreateInfo.imageExtent = SurfaceCapabilites.currentExtent;
	CreateInfo.imageFormat = ChosenSurfaceFormat.format;
	CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	CreateInfo.imageUsage = SurfaceCapabilites.supportedUsageFlags;
	CreateInfo.minImageCount = SurfaceCapabilites.minImageCount + 1;
	CreateInfo.oldSwapchain = VK_NULL_HANDLE;
	CreateInfo.presentMode = ChosenPresentMode;
	CreateInfo.preTransform = SurfaceCapabilites.currentTransform;

	if (vkCreateSwapchainKHR (GraphicsDevice, &CreateInfo, NULL, &Swapchain) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_SWAPCHAIN;
	}

	vkGetSwapchainImagesKHR (GraphicsDevice, Swapchain, &SwapchainImageCount, NULL);
	SwapchainImages.resize (SwapchainImageCount);
	vkGetSwapchainImagesKHR (GraphicsDevice, Swapchain, &SwapchainImageCount, SwapchainImages.data ());

	SwapchainImageViews.resize (SwapchainImageCount);
}

void Graphics::_CreateSwapchainImageViews ()
{
	OutputDebugString (L"Graphics::_CreateSwapchainImageViews\n");

	int i = 0;

	for (auto SwapchainImageView : SwapchainImageViews)
	{
		VkImageViewCreateInfo CreateInfo;
		memset (&CreateInfo, 0, sizeof (VkImageViewCreateInfo));

		VkComponentMapping Components;

		Components.a = VK_COMPONENT_SWIZZLE_A;
		Components.b = VK_COMPONENT_SWIZZLE_B;
		Components.g = VK_COMPONENT_SWIZZLE_G;
		Components.r = VK_COMPONENT_SWIZZLE_R;

		VkImageSubresourceRange SubresourceRange;

		SubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		SubresourceRange.baseMipLevel = 0;
		SubresourceRange.levelCount = 1;

		SubresourceRange.baseArrayLayer = 0;
		SubresourceRange.layerCount = 1;

		CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		CreateInfo.image = SwapchainImages[i];
		CreateInfo.format = ChosenSurfaceFormat.format;
		CreateInfo.components = Components;
		CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		CreateInfo.subresourceRange = SubresourceRange;

		if (vkCreateImageView (GraphicsDevice, &CreateInfo, NULL, &SwapchainImageView) != VK_SUCCESS)
		{
			throw GraphicsError::eCREATE_IMAGE_VIEW;
		}

		++i;
	}
}

Graphics::~Graphics ()
{
	OutputDebugString (L"Graphics::~Graphics\n");

	if (Swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR (GraphicsDevice, Swapchain, NULL);
	}

	for (auto SwapchainImageView : SwapchainImageViews)
	{
		if (SwapchainImageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView (GraphicsDevice, SwapchainImageView, NULL);
		}
	}

	if (GraphicsDevice != VK_NULL_HANDLE)
	{
		vkDestroyDevice (GraphicsDevice, NULL);
	}

	if (Surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR (Instance, Surface, NULL);
	}

	if (_IsValidationNeeded)
	{
		if (DebugUtilsMessenger != VK_NULL_HANDLE)
		{
			DestroyDebugUtilsMessenger (Instance, DebugUtilsMessenger, NULL);
		}
	}

	if (Instance != VK_NULL_HANDLE)
	{
		vkDestroyInstance (Instance, NULL);
	}
}
