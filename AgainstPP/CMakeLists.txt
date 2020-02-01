# CMakeList.txt : CMake project for AgainstCMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (${CMAKE_SOURCE_DIR})
#include_directories ($ENV{VULKAN_SDK}/Include)
include_directories ("E:\\Projects\\Vulkan-Hpp\\Vulkan-Headers\\include")
include_directories ("E:\\Projects\\Vulkan-Hpp\\vulkan")
include_directories (${CMAKE_SOURCE_DIR}/External/glm)
include_directories (${CMAKE_SOURCE_DIR}/External/tinygltf)
include_directories (${CMAKE_SOURCE_DIR}/External/stb)

link_directories ($ENV{VULKAN_SDK}/Lib32)
link_directories ($ENV{VULKAN_SDK}/Lib)

add_executable (AgainstPP WIN32 "template_event.hpp" "actor.cpp" "actor.hpp" "asset.cpp" "asset.hpp" "common_graphics.cpp" "common_graphics.hpp" "enums.hpp" "error.cpp" "error.hpp" "event.hpp" "event.cpp" "game.cpp" "game.hpp" "graphics_utils.cpp" "graphics_utils.hpp" "main.cpp" "scene.cpp" "scene.hpp" "main_menu.cpp" "main_menu.hpp" "splash_screen.cpp" "splash_screen.hpp" "splash_screen_graphics.cpp" "splash_screen_graphics.hpp" "utils.cpp" "utils.hpp")
add_compile_definitions (AgainstPP _CRT_SECURE_NO_WARNINGS UNICODE WIN32_LEAN_AND_MEAN VK_USE_PLATFORM_WIN32_KHR _HAS_EXCEPTIONS=0)
target_link_libraries (AgainstPP vulkan-1)
target_link_libraries (AgainstPP Shlwapi)

add_custom_command (TARGET AgainstPP POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/AgainstPP/Assets" $<TARGET_FILE_DIR:AgainstPP>)