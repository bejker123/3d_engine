#pragma once
#include <cstdint>
#include <optional>
#include <vulkan/vulkan.hpp>

#include "err.hpp"

namespace En {
namespace Vulkan {

struct QueueFamilyIndices {
  std::optional<uint32_t> graphics_family;
  std::optional<uint32_t> present_family;

  bool is_complete() {
    return graphics_family.has_value() && present_family.has_value();
  }
};

QueueFamilyIndices find_queue_families(VkPhysicalDevice dev,
                                       vk::SurfaceKHR surface);

bool is_dev_suitable(VkPhysicalDevice dev, vk::SurfaceKHR surface);

uint32_t rate_device(VkPhysicalDevice dev, vk::SurfaceKHR surface);

void dev_debug_info(VkPhysicalDevice &dev);

VulkanErr pick_phisical_dev(VkInstance &vk_instance,
                            VkPhysicalDevice &physical_dev,
                            vk::SurfaceKHR surface);

VulkanErr create_logical_device(VkPhysicalDevice &physical_dev,
                                VkDevice &device, VkQueue &graphics_queue,
                                vk::SurfaceKHR surface);

} // namespace Vulkan
} // namespace En
