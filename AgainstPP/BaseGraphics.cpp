#include <Windows.h>

#include "BaseGraphics.hpp"
#include "Error.hpp"

#include <vulkan/vulkan_win32.h>

PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT (VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
	return pfnVkCreateDebugUtilsMessengerEXT (instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT (VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks const* pAllocator)
{
	return pfnVkDestroyDebugUtilsMessengerEXT (instance, messenger, pAllocator);
}

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

BaseGraphics::BaseGraphics (HINSTANCE HInstance, HWND HWnd)
{
	OutputDebugString (L"BaseGraphics::BaseGraphics\n");

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
	_CreateSwapChain ();
}

void BaseGraphics::_PopulateInstanceLayersAndExtensions ()
{
	OutputDebugString (L"BaseGraphics::_PopulateInstanceLayersAndExtensions\n");

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

void BaseGraphics::_CreateInstance ()
{
	OutputDebugString (L"BaseGraphics::_CreateInstance\n");

	/*VkApplicationInfo ApplicationInfo = {};

	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.pEngineName = "AGE";
	ApplicationInfo.apiVersion = VK_API_VERSION_1_1;
	ApplicationInfo.applicationVersion = VK_MAKE_VERSION (1, 0, 0);
	ApplicationInfo.engineVersion = VK_MAKE_VERSION (1, 0, 0);
	ApplicationInfo.pApplicationName = "Against";

	VkInstanceCreateInfo CreateInfo = {};

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
	}*/

	vk::ApplicationInfo AI{ "Against", 1, "Against", 1, VK_API_VERSION_1_1 };
	vk::InstanceCreateInfo InstanceCreateInfo = { {}, &AI, RequestedInstanceLayerCount, RequestedInstanceLayers, RequestedInstanceExtensionCount, RequestedInstanceExtensions };

	Instance = vk::createInstanceUnique (InstanceCreateInfo);
}

void BaseGraphics::_SetupDebugUtilsMessenger ()
{
	OutputDebugString (L"BaseGraphics::_SetupDebugUtilsMessenger\n");

	/*VkDebugUtilsMessengerCreateInfoEXT CreateInfo = {};

	CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	CreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	CreateInfo.pfnUserCallback = DebugMessengerCallback;
	CreateInfo.flags = 0;

	if (CreateDebugUtilsMessenger (Instance, &CreateInfo, NULL, &DebugUtilsMessenger) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_DEBUG_UTILS_MESSENGER;
	}*/

	pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(Instance->getProcAddr ("vkCreateDebugUtilsMessengerEXT"));
	pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(Instance->getProcAddr ("vkDestroyDebugUtilsMessengerEXT"));

	vk::DebugUtilsMessageSeverityFlagsEXT severityFlags (vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
	vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
	DebugUtilsMessenger = Instance->createDebugUtilsMessengerEXTUnique (vk::DebugUtilsMessengerCreateInfoEXT ({}, severityFlags, messageTypeFlags, &DebugMessengerCallback));
}

void BaseGraphics::_GetPhysicalDevice ()
{
	OutputDebugString (L"BaseGraphics::_GetPhysicalDevice\n");

	PhysicalDevice = Instance->enumeratePhysicalDevices ().front ();
	uint32_t i = 0;
	for (auto QueueFamilyProperty : PhysicalDevice.getQueueFamilyProperties ())
	{
		if ((QueueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics) && QueueFamilyProperty.queueCount > 1)
		{
			GraphicsQueueFamilyIndex = i;

			break;
		}
		++i;
	}

	PhysicalDeviceLimits = PhysicalDevice.getProperties ().limits;

	/*uint32_t PhysicalDeviceCount = 0;
	vkEnumeratePhysicalDevices (Instance, &PhysicalDeviceCount, NULL);

	std::vector<VkPhysicalDevice> PhysicalDevices (PhysicalDeviceCount);
	vkEnumeratePhysicalDevices (Instance, &PhysicalDeviceCount, PhysicalDevices.data ());

	if (PhysicalDeviceCount == 0)
	{
		throw GraphicsError::eGET_PHYSICAL_DEVICE;
	}

	PhysicalDevice = PhysicalDevices[0];

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
	PhysicalDeviceLimits = DeviceProperties.limits;*/
}

void BaseGraphics::_CreateSurface (HINSTANCE HInstance, HWND HWnd)
{
	OutputDebugString (L"BaseGraphics::_CreateSurface\n");

	/*VkWin32SurfaceCreateInfoKHR CreateInfo = {};

	CreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	CreateInfo.hinstance = HInstance;
	CreateInfo.hwnd = HWnd;

	if (vkCreateWin32SurfaceKHR (Instance, &CreateInfo, NULL, &Surface) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_SURFACE;
	}*/

	Surface = Instance->createWin32SurfaceKHRUnique (vk::Win32SurfaceCreateInfoKHR ({}, HInstance, HWnd));
}

void BaseGraphics::_PopulateGraphicsDeviceExtensions ()
{
	OutputDebugString (L"BaseGraphics::_PopulateGraphicsDeviceExtensions\n");

	/*uint32_t ExtensionCount = 0;
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
	}*/

	for (auto ExtensionProperty : PhysicalDevice.enumerateDeviceExtensionProperties ())
	{
		if (strcmp (ExtensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			RequestedDeviceExtensions[RequestedDeviceExtensionCount++] = ExtensionProperty.extensionName;
			break;
		}
	}
}

void BaseGraphics::_CreateGraphicsDevice ()
{
	OutputDebugString (L"BaseGraphics::_CreateGraphicsDevice\n");

	float Priorities = 1.f;

	/*VkDeviceQueueCreateInfo QueueCreateInfo = {};

	QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	QueueCreateInfo.pNext = NULL;
	QueueCreateInfo.pQueuePriorities = &Priorities;
	QueueCreateInfo.queueCount = 1;
	QueueCreateInfo.queueFamilyIndex = GraphicsQueueFamilyIndex;
	QueueCreateInfo.flags = 0;

	VkDeviceCreateInfo DeviceCreateInfo = {};

	DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceCreateInfo.pNext = NULL;
	DeviceCreateInfo.enabledExtensionCount = RequestedDeviceExtensionCount;
	DeviceCreateInfo.ppEnabledExtensionNames = RequestedDeviceExtensions;
	DeviceCreateInfo.enabledLayerCount = 0;
	DeviceCreateInfo.ppEnabledLayerNames = NULL;
	DeviceCreateInfo.queueCreateInfoCount = 1;
	DeviceCreateInfo.pQueueCreateInfos = &QueueCreateInfo;
	DeviceCreateInfo.flags = 0;

	if (vkCreateDevice (PhysicalDevice, &DeviceCreateInfo, NULL, &GraphicsDevice) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_GRAPHICS_DEVICE;
	}

	vkGetDeviceQueue (GraphicsDevice, GraphicsQueueFamilyIndex, 0, &GraphicsQueue);*/

	vk::DeviceQueueCreateInfo QueueCreateInfo{ {}, GraphicsQueueFamilyIndex, 1, &Priorities };

	PhysicalDevice.createDeviceUnique (vk::DeviceCreateInfo ({}, 1, &QueueCreateInfo, 0, NULL, RequestedDeviceExtensionCount, RequestedDeviceExtensions));
}

void BaseGraphics::_CreateSwapChain ()
{
	OutputDebugString (L"BaseGraphics::_CreateSwapChain\n");

	/*VkBool32 IsSurfaceSupported = false;
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

	VkSwapchainCreateInfoKHR SwapchainCreateInfo = {};

	SwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapchainCreateInfo.surface = Surface;

	SwapchainCreateInfo.clipped = VK_TRUE;
	SwapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapchainCreateInfo.imageArrayLayers = 1;
	SwapchainCreateInfo.imageColorSpace = ChosenSurfaceFormat.colorSpace;
	SwapchainCreateInfo.imageExtent = SurfaceCapabilites.currentExtent;
	SwapchainCreateInfo.imageFormat = ChosenSurfaceFormat.format;
	SwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	SwapchainCreateInfo.imageUsage = SurfaceCapabilites.supportedUsageFlags;
	SwapchainCreateInfo.minImageCount = SurfaceCapabilites.minImageCount + 1;
	SwapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	SwapchainCreateInfo.presentMode = ChosenPresentMode;
	SwapchainCreateInfo.preTransform = SurfaceCapabilites.currentTransform;

	if (vkCreateSwapchainKHR (GraphicsDevice, &SwapchainCreateInfo, NULL, &Swapchain) != VK_SUCCESS)
	{
		throw GraphicsError::eCREATE_SWAPCHAIN;
	}

	vkGetSwapchainImagesKHR (GraphicsDevice, Swapchain, &SwapchainImageCount, NULL);
	SwapchainImages.resize (SwapchainImageCount);
	vkGetSwapchainImagesKHR (GraphicsDevice, Swapchain, &SwapchainImageCount, SwapchainImages.data ());

	SwapchainImageViews.resize (SwapchainImageCount);*/

	if (!PhysicalDevice.getSurfaceSupportKHR (GraphicsQueueFamilyIndex, Surface.get ()))
	{
		throw GraphicsError::eSURFACE_SUPPORT;
	}

	for (auto SurfaceFormat : PhysicalDevice.getSurfaceFormatsKHR (Surface.get ()))
	{
		if (SurfaceFormat.format == vk::Format::eR8G8B8A8Unorm)
		{
			ChosenSurfaceFormat = SurfaceFormat.format;
			break;
		}
	}

	for (auto PresentMode : PhysicalDevice.getSurfacePresentModesKHR (Surface.get ()))
	{
		if (PresentMode == vk::PresentModeKHR::eMailbox)
		{
			ChosenPresentMode = PresentMode;
		}
	}

	vk::SurfaceCapabilitiesKHR SurfaceCapabilities = PhysicalDevice.getSurfaceCapabilitiesKHR (Surface.get ());
	SurfaceExtent = SurfaceCapabilities.currentExtent;

	vk::SwapchainCreateInfoKHR SwapchainCreateInfo{ {}, Surface.get (), SurfaceCapabilities.minImageCount + 1, ChosenSurfaceFormat.format, ChosenSurfaceFormat.colorSpace, SurfaceExtent, 1, SurfaceCapabilities.supportedUsageFlags, vk::SharingMode::eExclusive, 1, &GraphicsQueueFamilyIndex, SurfaceCapabilities.currentTransform, vk::CompositeAlphaFlagBitsKHR::eOpaque, ChosenPresentMode, VK_TRUE, VK_NULL_HANDLE };
	Swapchain = GraphicsDevice.createSwapchainKHRUnique (SwapchainCreateInfo);

	SwapchainImages = GraphicsDevice.getSwapchainImagesKHR (Swapchain.get ());

	vk::ComponentMapping Components (vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
	vk::ImageSubresourceRange SubresourceRange (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

	vk::ImageViewCreateInfo SwapchainImageViewCreateInfo ({}, nullptr, vk::ImageViewType::e2D, ChosenSurfaceFormat.format, Components, SubresourceRange);

	int i = 0;
	for (auto SwapchainImageView : SwapchainImageViews)
	{
		SwapchainImageViewCreateInfo.image = SwapchainImages[i];

		SwapchainImageViews.push_back (GraphicsDevice.createImageView (SwapchainImageViewCreateInfo));

		++i;
	}
}

BaseGraphics::~BaseGraphics ()
{
	OutputDebugString (L"BaseGraphics::~BaseGraphics\n");

	/*if (Swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR (GraphicsDevice, Swapchain, NULL);
	}

	for (auto& SwapchainImageView : SwapchainImageViews)
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
	}*/
}
