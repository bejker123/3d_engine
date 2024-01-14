#pragma once
#include <vector>
constexpr bool ENABLE_VALIDATION_LAYERS = true;
const std::vector<const char *> validation_layers = {
    "VK_LAYER_KHRONOS_validation"}; //, "VK_LAYER_LUNARG_api_dump"};
