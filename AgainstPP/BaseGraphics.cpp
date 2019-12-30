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
		std::vector<vk::LayerProperties> LayerProperties = vk::enumerateInstanceLayerProperties ();
		std::vector<vk::LayerProperties>::iterator It = std::find_if (LayerProperties.begin (), LayerProperties.end (), [](const vk::LayerProperties& LayerProperty) { return strcmp (LayerProperty.layerName, "VK_LAYER_LUNARG_standard_validation") == 0; });

		RequestedInstanceLayers.push_back ("VK_LAYER_LUNARG_standard_validation");
	}

	std::vector<vk::ExtensionProperties> ExtensionProperties = vk::enumerateInstanceExtensionProperties ();
	std::vector<vk::ExtensionProperties>::iterator It = std::find_if (ExtensionProperties.begin (), ExtensionProperties.end (), [](const vk::ExtensionProperties& ExtensionProperty) { return strcmp (ExtensionProperty.extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0; });
	RequestedInstanceExtensions.push_back (VK_KHR_SURFACE_EXTENSION_NAME);

	It = std::find_if (ExtensionProperties.begin (), ExtensionProperties.end (), [](const vk::ExtensionProperties& ExtensionProperty) { return strcmp (ExtensionProperty.extensionName, "VK_KHR_win32_surface") == 0; });
	RequestedInstanceExtensions.push_back ("VK_KHR_win32_surface");

	if (_IsValidationNeeded)
	{
		It = std::find_if (ExtensionProperties.begin (), ExtensionProperties.end (), [](const vk::ExtensionProperties& ExtensionProperty) { return strcmp (ExtensionProperty.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0; });
		RequestedInstanceExtensions.push_back (VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
}

void BaseGraphics::_CreateInstance ()
{
	OutputDebugString (L"BaseGraphics::_CreateInstance\n");

	vk::ApplicationInfo AI ("Against", 1, "Against", 1, VK_API_VERSION_1_1);
	vk::InstanceCreateInfo InstanceCreateInfo ({}, &AI, RequestedInstanceLayers.size (), RequestedInstanceLayers.data (), RequestedInstanceExtensions.size (), RequestedInstanceExtensions.data ());

	Instance = vk::createInstance (InstanceCreateInfo);
}

void BaseGraphics::_SetupDebugUtilsMessenger ()
{
	OutputDebugString (L"BaseGraphics::_SetupDebugUtilsMessenger\n");

	pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(Instance.getProcAddr ("vkCreateDebugUtilsMessengerEXT"));
	pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(Instance.getProcAddr ("vkDestroyDebugUtilsMessengerEXT"));

	vk::DebugUtilsMessageSeverityFlagsEXT severityFlags (vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);// | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
	vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
	DebugUtilsMessenger = Instance.createDebugUtilsMessengerEXT (vk::DebugUtilsMessengerCreateInfoEXT ({}, severityFlags, messageTypeFlags, &DebugMessengerCallback));
}

void BaseGraphics::_GetPhysicalDevice ()
{
	OutputDebugString (L"BaseGraphics::_GetPhysicalDevice\n");

	PhysicalDevice = Instance.enumeratePhysicalDevices ().front ();

	uint32_t i = 0;
	for (auto QueueFamilyProperty : PhysicalDevice.getQueueFamilyProperties ())
	{
		if ((QueueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics) && QueueFamilyProperty.queueCount > 1)
		{
			GraphicsQueueFamilies.push_back (i);
			break;
		}

		++i;
	}

	PhysicalDeviceLimits = PhysicalDevice.getProperties ().limits;
	PhysicalDeviceMemoryProperties = PhysicalDevice.getMemoryProperties ();
}

void BaseGraphics::_CreateSurface (HINSTANCE HInstance, HWND HWnd)
{
	OutputDebugString (L"BaseGraphics::_CreateSurface\n");

	Surface = Instance.createWin32SurfaceKHR (vk::Win32SurfaceCreateInfoKHR ({}, HInstance, HWnd));
}

void BaseGraphics::_PopulateGraphicsDeviceExtensions ()
{
	OutputDebugString (L"BaseGraphics::_PopulateGraphicsDeviceExtensions\n");

	std::vector<vk::ExtensionProperties> ExtensionProperties = PhysicalDevice.enumerateDeviceExtensionProperties ();
	std::vector< vk::ExtensionProperties>::iterator It = std::find_if (ExtensionProperties.begin (), ExtensionProperties.end (), [](const vk::ExtensionProperties& ExtensionProperty) { return strcmp (ExtensionProperty.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });

	RequestedDeviceExtensions.push_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void BaseGraphics::_CreateGraphicsDevice ()
{
	OutputDebugString (L"BaseGraphics::_CreateGraphicsDevice\n");

	float Priorities = 1.f;

	vk::DeviceQueueCreateInfo QueueCreateInfo ({}, GraphicsQueueFamilies[0], GraphicsQueueFamilies.size (), &Priorities);
	vk::DeviceCreateInfo DeviceCreateInfo ({}, 1, &QueueCreateInfo, 0, NULL, RequestedDeviceExtensions.size (), RequestedDeviceExtensions.data ());

	GraphicsDevice = PhysicalDevice.createDevice (DeviceCreateInfo);
}

void BaseGraphics::_CreateSwapChain ()
{
	OutputDebugString (L"BaseGraphics::_CreateSwapChain\n");

	if (!PhysicalDevice.getSurfaceSupportKHR (GraphicsQueueFamilies[0], Surface))
	{
		throw GraphicsError::eSURFACE_SUPPORT;
	}

	for (auto SurfaceFormat : PhysicalDevice.getSurfaceFormatsKHR (Surface))
	{
		if (SurfaceFormat.format == vk::Format::eB8G8R8A8Unorm)
		{
			ChosenSurfaceFormat = SurfaceFormat.format;
			break;
		}
	}

	for (auto PresentMode : PhysicalDevice.getSurfacePresentModesKHR (Surface))
	{
		if (PresentMode == vk::PresentModeKHR::eMailbox)
		{
			ChosenPresentMode = PresentMode;
		}
	}

	vk::SurfaceCapabilitiesKHR SurfaceCapabilities = PhysicalDevice.getSurfaceCapabilitiesKHR (Surface);
	SurfaceExtent = SurfaceCapabilities.currentExtent;

	vk::SwapchainCreateInfoKHR SwapchainCreateInfo{ {}, Surface, SurfaceCapabilities.minImageCount + 1, ChosenSurfaceFormat.format, ChosenSurfaceFormat.colorSpace, SurfaceExtent, 1, SurfaceCapabilities.supportedUsageFlags, vk::SharingMode::eExclusive, 1, &GraphicsQueueFamilies[0], SurfaceCapabilities.currentTransform, vk::CompositeAlphaFlagBitsKHR::eOpaque, ChosenPresentMode, VK_TRUE, VK_NULL_HANDLE };
	Swapchain = GraphicsDevice.createSwapchainKHR (SwapchainCreateInfo);

	SwapchainImages = GraphicsDevice.getSwapchainImagesKHR (Swapchain);

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

	if (Swapchain != VK_NULL_HANDLE)
	{
		GraphicsDevice.destroySwapchainKHR (Swapchain);
	}

	for (auto& SwapchainImageView : SwapchainImageViews)
	{
		if (SwapchainImageView != VK_NULL_HANDLE)
		{
			GraphicsDevice.destroyImageView (SwapchainImageView);
		}
	}

	if (GraphicsDevice != VK_NULL_HANDLE)
	{
		GraphicsDevice.destroy ();
	}

	if (Surface != VK_NULL_HANDLE)
	{
		Instance.destroySurfaceKHR (Surface);
	}

	if (_IsValidationNeeded)
	{
		if (DebugUtilsMessenger != VK_NULL_HANDLE)
		{
			Instance.destroyDebugUtilsMessengerEXT (DebugUtilsMessenger);
		}
	}

	if (Instance != VK_NULL_HANDLE)
	{
		Instance.destroy ();
	}
}
