#include "Error.hpp"
#include <Windows.h>

void LogError (GraphicsError Err)
{
	switch (Err)
	{
	case GraphicsError::ePOPULATE_INSTANCE_LAYERS_AND_EXTENSIONS:
		OutputDebugString (L"Graphics Error: Populating Instance Layers and Extensions\n");
		break;

	case GraphicsError::eCREATE_INSTANCE:
		OutputDebugString (L"Graphics Error: Create Instance\n");
		break;

	case GraphicsError::eSETUP_DEBUG_UTILS_MESSENGER:
		OutputDebugString (L"Graphics Error: Setup Debug Utils Messenger\n");
		break;

	case GraphicsError::eCREATE_DEBUG_UTILS_MESSENGER:
		OutputDebugString (L"Graphics Error: Create Debug Utils Messenger\n");
		break;

	case GraphicsError::eDESTROY_DEBUG_UTILS_MESSENGER:
		OutputDebugString (L"Graphics Error: Destroy Debud Utils Messenger\n");
		break;

	case GraphicsError::eCREATE_SURFACE:
		OutputDebugString (L"Graphics Error: Create Surface\n");
		break;

	case GraphicsError::eGET_PHYSICAL_DEVICE:
		OutputDebugString (L"Graphics Error: Get Physical Device\n");
		break;

	case GraphicsError::ePOPULATE_DEVICE_LAYERS_AND_EXTENSIONS:
		OutputDebugString (L"Graphics Error: Populate Device Layers and Extensions\n");
		break;

	case GraphicsError::eCREATE_GRAPHICS_DEVICE:
		OutputDebugString (L"Graphics Error: Create Graphics Device\n");
		break;

	case GraphicsError::eSURFACE_SUPPORT:
		OutputDebugString (L"Graphics Error: Surface Support\n");
		break;

	case GraphicsError::eCREATE_SWAPCHAIN:
		OutputDebugString (L"Graphics Error: Create Swapchain\n");
		break;

	case GraphicsError::eCREATE_IMAGE_VIEW:
		OutputDebugString (L"Graphics Error: Create Image View\n");
		break;

	case GraphicsError::eCREATE_BUFFER:
		OutputDebugString (L"Graphics Error: Create Buffer\n");
		break;

	case GraphicsError::eALLOCATE_MEMORY:
		OutputDebugString (L"Graphics Error: Allocate Buffer Memory\n");
		break;

	case GraphicsError::eBIND_BUFFER_MEMORY:
		OutputDebugString (L"Graphics Error: Bind Buffer Memory\n");
		break;

	case GraphicsError::eCREATE_IMAGE:
		OutputDebugString (L"Graphics Error: Create Image\n");
		break;

	case GraphicsError::eBIND_IMAGE_MEMORY:
		OutputDebugString (L"Graphics Error: Bind Image Memory\n");
		break;

	case GraphicsError::eMAP_MEMORY:
		OutputDebugString (L"Graphics Error: Map Image Memory\n");
		break;

	case GraphicsError::eCREATE_DESCRIPTOR_SET_LAYOUT:
		OutputDebugString (L"Graphics Error: Create Descriptor Set Layout\n");
		break;

	case GraphicsError::eCREATE_PIPELINE_LAYOUT:
		OutputDebugString (L"Graphics Error: Create Pipeline Layout\n");
		break;

	case GraphicsError::eCREATE_DESCRIPTOR_POOL:
		OutputDebugString (L"Graphics Error: Create Descriptor Pool\n");
		break;

	case GraphicsError::eALLOCATE_DESCRIPTOR_SET:
		OutputDebugString (L"Graphics Error: Allocate Descriptor Set\n");
		break;

	case GraphicsError::eCREATE_RENDER_PASS:
		OutputDebugString (L"Graphics Error: Create Render Pass\n");
		break;

	case GraphicsError::eCREATE_SHADER_MODULE:
		OutputDebugString (L"Graphics Error: Create Shader Module\n");
		break;

	case GraphicsError::eCREATE_FRAMEBUFFER:
		OutputDebugString (L"Graphics Error: Create Framebuffer\n");
		break;

	case GraphicsError::eBEGIN_COMMAND_BUFFER:
		OutputDebugString (L"Graphics Error: Begin Command Buffer\n");
		break;

	case GraphicsError::eEND_COMMAND_BUFFER:
		OutputDebugString (L"Graphics Error: End Command Buffer\n");
		break;

	case GraphicsError::eCREATE_COMMAND_POOL:
		OutputDebugString (L"Graphics Error: Create Command Pool\n");
		break;

	case GraphicsError::eALLOCATE_COMMAND_BUFFER:
		OutputDebugString (L"Graphics Error: Allocate Command Buffer\n");
		break;

	case GraphicsError::eCREATE_GRAPHICS_PIPELINE:
		OutputDebugString (L"Graphics Error: Create Graphics Pipeline\n");
		break;

	case GraphicsError::eCREATE_SEMAPHORE:
		OutputDebugString (L"Graphics Error: Create Semaphore\n");
		break;

	case GraphicsError::eACQUIRE_NEXT_IMAGE:
		OutputDebugString (L"Graphics Error: Acquire Next Image\n");
		break;

	case GraphicsError::eWAIT_FOR_FENCES:
		OutputDebugString (L"Graphics Error: Wait for Fence\n");
		break;

	case GraphicsError::eCREATE_FENCE:
		OutputDebugString (L"Graphics Error: Create Fence\n");
		break;

	case GraphicsError::eRESET_FENCE:
		OutputDebugString (L"Graphics Error: Reset Fence\n");
		break;

	case GraphicsError::eQUEUE_SUBMIT:
		OutputDebugString (L"Graphics Error: Queue Submit\n");
		break;

	case GraphicsError::eQUEUE_PRESENT:
		OutputDebugString (L"Graphics Error: Queue Present\n");
		break;

	case GraphicsError::eUPDATE_UNIFORM_BUFFER:
		OutputDebugString (L"Graphics Error: Update Uniform Buffer\n");
		break;

	case GraphicsError::eCREATE_SAMPLER:
		OutputDebugString (L"Graphics Error: Create Texture SplashScreenSampler\n");
		break;

	default:
		break;
	}
}

void LogError (GLTFError Err)
{
	switch (Err)
	{
	case GLTFError::eCouldNotImport:
		OutputDebugString (L"GLTF Error: Could not import\n");
		break;
	default:
		break;
	}
}