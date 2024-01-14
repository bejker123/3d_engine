#pragma once
#include <cstdint>
#include <optional>
#include <vulkan/vulkan.hpp>

#include "err.hpp"

namespace En {
namespace Vulkan {

struct QueueFamilyIndices {
  std::optional<uint32_t> graphics_family;

  bool is_complete() { return graphics_family.has_value(); }
};

QueueFamilyIndices find_queue_families(VkPhysicalDevice dev);

bool is_dev_suitable(VkPhysicalDevice dev);

uint32_t rate_device(VkPhysicalDevice dev);

void dev_debug_info(VkPhysicalDevice &dev);

VulkanErr pick_phisical_dev(VkInstance &vk_instance,
                            VkPhysicalDevice &physical_dev);

VulkanErr create_logical_device(VkPhysicalDevice &physical_dev,
                                VkDevice &device, VkQueue &graphics_queue);

} // namespace Vulkan
} // namespace En
