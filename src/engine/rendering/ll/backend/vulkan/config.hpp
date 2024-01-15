#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>
constexpr bool ENABLE_VALIDATION_LAYERS = true;
const std::vector<const char *> validation_layers = {
    "VK_LAYER_KHRONOS_validation"}; //, "VK_LAYER_LUNARG_api_dump"};

const std::vector<const char *> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};
