#pragma once
#include "vulkan/err.hpp"
#define GLFW_INCLUDE_NONE // Don't include the OpenGL header
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
namespace En {
namespace Vulkan {

VulkanErr init(GLFWwindow *window);

void terminate();

}; // namespace Vulkan
} // namespace En
