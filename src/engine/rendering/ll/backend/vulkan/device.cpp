#include "device.hpp"
#include "config.hpp"
#include <map>
#include <vulkan/vulkan_handles.hpp>

#define LOGGER_PREFIX "[VULKAN] "
#include "../../../../io/logger.hpp"
namespace En {
namespace Vulkan {
QueueFamilyIndices find_queue_families(VkPhysicalDevice dev,
                                       vk::SurfaceKHR surface) {
  QueueFamilyIndices indices;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count,
                                           queue_families.data());

  size_t i = 0;
  for (const auto &family : queue_families) {
    if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphics_family = i;
    }
    VkBool32 present_support;
    vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &present_support);
    if (present_support) {
      indices.present_family = i;
    }
    if (indices.is_complete()) {
      break;
    }
    i++;
  }

  return indices;
}

bool is_dev_suitable(VkPhysicalDevice dev, vk::SurfaceKHR surface) {
  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceFeatures(dev, &features);

  auto indices = find_queue_families(dev, surface);

  return features.geometryShader && indices.is_complete();
}

uint32_t rate_device(VkPhysicalDevice dev, vk::SurfaceKHR surface) {
  VkPhysicalDeviceProperties prop;
  vkGetPhysicalDeviceProperties(dev, &prop);

  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceFeatures(dev, &features);

  if (!is_dev_suitable(dev, surface)) {
    return 0;
  }

  uint32_t score = 0;
  if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  score += prop.limits.maxImageDimension2D;

  return score;
}

void dev_debug_info(VkPhysicalDevice &dev) {
  VkPhysicalDeviceProperties prop;
  vkGetPhysicalDeviceProperties(dev, &prop);

  VkPhysicalDeviceMemoryProperties mem_prop;
  vkGetPhysicalDeviceMemoryProperties(dev, &mem_prop);

  LOG("Device picked:\n\t%s %ldGB\n", prop.deviceName,
      mem_prop.memoryHeaps[0].size / 1024 / 1024 / 1024);
}

VulkanErr pick_phisical_dev(VkInstance &vk_instance,
                            VkPhysicalDevice &physical_dev,
                            vk::SurfaceKHR surface) {

  uint32_t devs_count = 0;
  vkEnumeratePhysicalDevices(vk_instance, &devs_count, nullptr);

  if (devs_count == 0) {
    LOG("Failed to find any GPUs with Vulkan support.");
    return VulkanError::NO_VULKAN_DEVICE;
  }

  std::vector<VkPhysicalDevice> devices(devs_count);
  vkEnumeratePhysicalDevices(vk_instance, &devs_count, devices.data());

  // score, device
  std::multimap<uint32_t, VkPhysicalDevice> candidates;

  for (const auto &dev : devices) {
    candidates.insert(std::make_pair(rate_device(dev, surface), dev));
  }

  if (candidates.rbegin()->first > 0) {
    physical_dev = candidates.rbegin()->second;
    dev_debug_info(physical_dev);
  } else {
    LOG("Failed to find any suitable device.");
    return VulkanError::NO_SUITABLE_DEVICE;
  }

  return std::nullopt;
}

VulkanErr create_logical_device(VkPhysicalDevice &physical_dev,
                                VkDevice &device, VkQueue &graphics_queue,
                                vk::SurfaceKHR surface) {
  QueueFamilyIndices indices = find_queue_families(physical_dev, surface);
  VkDeviceQueueCreateInfo queue_createinfo{};
  queue_createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_createinfo.queueFamilyIndex = indices.graphics_family.value();
  queue_createinfo.queueCount = 1;

  float queue_priority = 1.0;
  queue_createinfo.pQueuePriorities = &queue_priority;

  VkPhysicalDeviceFeatures features{};
  VkDeviceCreateInfo createinfo{};
  createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createinfo.pQueueCreateInfos = &queue_createinfo;
  createinfo.queueCreateInfoCount = 1;

  createinfo.pEnabledFeatures = &features;

  if (ENABLE_VALIDATION_LAYERS) {
    createinfo.enabledLayerCount =
        static_cast<uint32_t>(validation_layers.size());
    createinfo.ppEnabledLayerNames = validation_layers.data();
  } else {
    createinfo.enabledExtensionCount = 0;
  }

  if (vkCreateDevice(physical_dev, &createinfo, nullptr, &device) !=
      VK_SUCCESS) {
    return VulkanError::CREATE_DEVICE;
  }

  vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &graphics_queue);

  return std::nullopt;
}

} // namespace Vulkan
} // namespace En
