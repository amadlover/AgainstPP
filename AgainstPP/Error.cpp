#include "Error.hpp"
#include <Windows.h>

void LogError (GraphicsError Err)
{
	switch (Err)
	{
	case GraphicsError::ePOPULATE_INSTANCE_LAYERS_AND_EXTENSIONS:
		OutputDebugString (L"BaseGraphics Error: Populating Instance Layers and Extensions\n");
		break;

	case GraphicsError::eCREATE_INSTANCE:
		OutputDebugString (L"BaseGraphics Error: Create Instance\n");
		break;

	case GraphicsError::eSETUP_DEBUG_UTILS_MESSENGER:
		OutputDebugString (L"BaseGraphics Error: Setup Debug Utils Messenger\n");
		break;

	case GraphicsError::eCREATE_DEBUG_UTILS_MESSENGER:
		OutputDebugString (L"BaseGraphics Error: Create Debug Utils Messenger\n");
		break;

	case GraphicsError::eDESTROY_DEBUG_UTILS_MESSENGER:
		OutputDebugString (L"BaseGraphics Error: Destroy Debud Utils Messenger\n");
		break;

	case GraphicsError::eCREATE_SURFACE:
		OutputDebugString (L"BaseGraphics Error: Create Surface\n");
		break;

	case GraphicsError::eGET_PHYSICAL_DEVICE:
		OutputDebugString (L"BaseGraphics Error: Get Physical Device\n");
		break;

	case GraphicsError::ePOPULATE_DEVICE_LAYERS_AND_EXTENSIONS:
		OutputDebugString (L"BaseGraphics Error: Populate Device Layers and Extensions\n");
		break;

	case GraphicsError::eCREATE_GRAPHICS_DEVICE:
		OutputDebugString (L"BaseGraphics Error: Create BaseGraphics Device\n");
		break;

	case GraphicsError::eSURFACE_SUPPORT:
		OutputDebugString (L"BaseGraphics Error: Surface Support\n");
		break;

	case GraphicsError::eCREATE_SWAPCHAIN:
		OutputDebugString (L"BaseGraphics Error: Create Swapchain\n");
		break;

	case GraphicsError::eCREATE_IMAGE_VIEW:
		OutputDebugString (L"BaseGraphics Error: Create Image View\n");
		break;

	case GraphicsError::eCREATE_BUFFER:
		OutputDebugString (L"BaseGraphics Error: Create Buffer\n");
		break;

	case GraphicsError::eALLOCATE_MEMORY:
		OutputDebugString (L"BaseGraphics Error: Allocate Buffer Memory\n");
		break;

	case GraphicsError::eBIND_BUFFER_MEMORY:
		OutputDebugString (L"BaseGraphics Error: Bind Buffer Memory\n");
		break;

	case GraphicsError::eCREATE_IMAGE:
		OutputDebugString (L"BaseGraphics Error: Create Image\n");
		break;

	case GraphicsError::eBIND_IMAGE_MEMORY:
		OutputDebugString (L"BaseGraphics Error: Bind Image Memory\n");
		break;

	case GraphicsError::eMAP_MEMORY:
		OutputDebugString (L"BaseGraphics Error: Map Image Memory\n");
		break;

	case GraphicsError::eCREATE_DESCRIPTOR_SET_LAYOUT:
		OutputDebugString (L"BaseGraphics Error: Create Descriptor Set Layout\n");
		break;

	case GraphicsError::eCREATE_PIPELINE_LAYOUT:
		OutputDebugString (L"BaseGraphics Error: Create Pipeline Layout\n");
		break;

	case GraphicsError::eCREATE_DESCRIPTOR_POOL:
		OutputDebugString (L"BaseGraphics Error: Create Descriptor Pool\n");
		break;

	case GraphicsError::eALLOCATE_DESCRIPTOR_SET:
		OutputDebugString (L"BaseGraphics Error: Allocate Descriptor Set\n");
		break;

	case GraphicsError::eCREATE_RENDER_PASS:
		OutputDebugString (L"BaseGraphics Error: Create Render Pass\n");
		break;

	case GraphicsError::eCREATE_SHADER_MODULE:
		OutputDebugString (L"BaseGraphics Error: Create Shader Module\n");
		break;

	case GraphicsError::eCREATE_FRAMEBUFFER:
		OutputDebugString (L"BaseGraphics Error: Create Framebuffer\n");
		break;

	case GraphicsError::eBEGIN_COMMAND_BUFFER:
		OutputDebugString (L"BaseGraphics Error: Begin Command Buffer\n");
		break;

	case GraphicsError::eEND_COMMAND_BUFFER:
		OutputDebugString (L"BaseGraphics Error: End Command Buffer\n");
		break;

	case GraphicsError::eCREATE_COMMAND_POOL:
		OutputDebugString (L"BaseGraphics Error: Create Command Pool\n");
		break;

	case GraphicsError::eALLOCATE_COMMAND_BUFFER:
		OutputDebugString (L"BaseGraphics Error: Allocate Command Buffer\n");
		break;

	case GraphicsError::eCREATE_GRAPHICS_PIPELINE:
		OutputDebugString (L"BaseGraphics Error: Create BaseGraphics Pipeline\n");
		break;

	case GraphicsError::eCREATE_SEMAPHORE:
		OutputDebugString (L"BaseGraphics Error: Create Semaphore\n");
		break;

	case GraphicsError::eACQUIRE_NEXT_IMAGE:
		OutputDebugString (L"BaseGraphics Error: Acquire Next Image\n");
		break;

	case GraphicsError::eWAIT_FOR_FENCES:
		OutputDebugString (L"BaseGraphics Error: Wait for Fence\n");
		break;

	case GraphicsError::eCREATE_FENCE:
		OutputDebugString (L"BaseGraphics Error: Create Fence\n");
		break;

	case GraphicsError::eRESET_FENCE:
		OutputDebugString (L"BaseGraphics Error: Reset Fence\n");
		break;

	case GraphicsError::eQUEUE_SUBMIT:
		OutputDebugString (L"BaseGraphics Error: Queue Submit\n");
		break;

	case GraphicsError::eQUEUE_PRESENT:
		OutputDebugString (L"BaseGraphics Error: Queue Present\n");
		break;

	case GraphicsError::eUPDATE_UNIFORM_BUFFER:
		OutputDebugString (L"BaseGraphics Error: Update Uniform Buffer\n");
		break;

	case GraphicsError::eCREATE_SAMPLER:
		OutputDebugString (L"BaseGraphics Error: Create Texture SplashScreenSampler\n");
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