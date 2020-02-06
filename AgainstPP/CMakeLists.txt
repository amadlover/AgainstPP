# CMakeList.txt : CMake project for AgainstCMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (${CMAKE_SOURCE_DIR})
include_directories ($ENV{VULKAN_SDK}/Include)
include_directories (${CMAKE_SOURCE_DIR}/External/glm)
include_directories (${CMAKE_SOURCE_DIR}/External/tinygltf)
include_directories (${CMAKE_SOURCE_DIR}/External/stb)
include_directories (${CMAKE_SOURCE_DIR}/External/imgui)
include_directories (${CMAKE_SOURCE_DIR}/External/imgui/examples)

link_directories ($ENV{VULKAN_SDK}/Lib32)
link_directories ($ENV{VULKAN_SDK}/Lib)

set (IMGUI_HPP ${CMAKE_SOURCE_DIR}/External/imgui/imgui.h ${CMAKE_SOURCE_DIR}/External/imgui/imconfig.h ${CMAKE_SOURCE_DIR}/External/imgui/imgui_internal.h ${CMAKE_SOURCE_DIR}/External/imgui/imstb_rectpack.h ${CMAKE_SOURCE_DIR}/External/imgui/imstb_textedit.h ${CMAKE_SOURCE_DIR}/External/imgui/imstb_truetype.h ${CMAKE_SOURCE_DIR}/External/imgui/examples/imgui_impl_win32.h ${CMAKE_SOURCE_DIR}/External/imgui/examples/imgui_impl_vulkan.h)
set (IMGUI_CPP ${CMAKE_SOURCE_DIR}/External/imgui/imgui.cpp ${CMAKE_SOURCE_DIR}/External/imgui/imgui_demo.cpp ${CMAKE_SOURCE_DIR}/External/imgui/imgui_draw.cpp ${CMAKE_SOURCE_DIR}/External/imgui/imgui_widgets.cpp ${CMAKE_SOURCE_DIR}/External/imgui/examples/imgui_impl_win32.cpp ${CMAKE_SOURCE_DIR}/External/imgui/examples/imgui_impl_vulkan.cpp)
set (HPP "entity.hpp" "actor.hpp" "asset.hpp" "common_graphics.hpp" "enums.hpp" "error.hpp" "event.hpp" "game.hpp" "graphics_utils.hpp" "scene.hpp" "main_menu.hpp" "splash_screen.hpp" "splash_screen_graphics.hpp" "scene_graphics.hpp" "utils.hpp")
set (CPP "entity.cpp" "actor.cpp" "asset.cpp" "common_graphics.cpp" "error.cpp" "game.cpp" "graphics_utils.cpp" "main.cpp" "scene.cpp" "main_menu.cpp" "splash_screen.cpp" "splash_screen_graphics.cpp" "scene_graphics.cpp" "utils.cpp")
set (GLSL "Assets/Shaders/UI.vert" "Assets/Shaders/UI.frag" "Assets/Shaders/pbr.vert" "Assets/Shaders/pbr.frag")

source_group ("Shaders" GLSL)

add_executable (AgainstPP WIN32 ${HPP} ${CPP} ${IMGUI_HPP} ${IMGUI_CPP})
add_compile_definitions (AgainstPP _CRT_SECURE_NO_WARNINGS UNICODE WIN32_LEAN_AND_MEAN VK_USE_PLATFORM_WIN32_KHR _HAS_EXCEPTIONS=0)
target_link_libraries (AgainstPP vulkan-1)
target_link_libraries (AgainstPP Shlwapi)

add_custom_command (TARGET AgainstPP POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/AgainstPP/Assets" $<TARGET_FILE_DIR:AgainstPP>)