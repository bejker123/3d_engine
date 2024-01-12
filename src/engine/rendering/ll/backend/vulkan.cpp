#include "vulkan.hpp"
#include "../../../io/logger.hpp"
#include <iostream>
#include <map>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#define LOGGER_PREFIX "[VULKAN] "

namespace En {
namespace Vulkan {
constexpr bool ENABLE_VALIDATION_LAYERS = true;

static VkInstance vk_instance;
// static std::shared_ptr<Window> window;
VkDebugUtilsMessengerEXT dbg_messenger;

VkPhysicalDevice physical_dev;

bool check_vl_support();
const std::vector<const char *> validation_layers = {
    "VK_LAYER_KHRONOS_validation"}; //, "VK_LAYER_LUNARG_api_dump"};

std::vector<const char *> get_req_extentions() {
  uint32_t ext_count = 0;
  const char **exts = glfwGetRequiredInstanceExtensions(&ext_count);

  std::vector<const char *> extentions(exts, exts + ext_count);

  if (ENABLE_VALIDATION_LAYERS) {
    extentions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extentions;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *cb_data, void *user_data) {
  std::cerr << "Layer: " << cb_data->pMessage << std::endl;

  return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

void populate_dbg_messenger(VkDebugUtilsMessengerCreateInfoEXT &createinfo) {
  createinfo = {};
  createinfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createinfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createinfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createinfo.pfnUserCallback = debug_callback;
  createinfo.pUserData = nullptr; // This is optional
}
std::optional<VulkanError> setup_dbg_messenger() {
  if (!ENABLE_VALIDATION_LAYERS) {
    return std::nullopt;
  }

  VkDebugUtilsMessengerCreateInfoEXT createinfo;
  populate_dbg_messenger(createinfo);

  if (CreateDebugUtilsMessengerEXT(vk_instance, &createinfo, nullptr,
                                   &dbg_messenger) != VK_SUCCESS) {
    LOG("Failed to set up debug messenger.\n");
    return VulkanError::SETUP_DBG_MESSENGER;
  }
  return std::nullopt;
}

std::optional<VulkanError> create_vk_instance() {
  if (ENABLE_VALIDATION_LAYERS && !check_vl_support()) {
    LOG("No validation layers available.\n");
    return VulkanError::NO_VALIDATION_LAYERS;
  }

  VkApplicationInfo appinfo{};
  appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appinfo.pApplicationName = "3d_engine";
  appinfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  appinfo.pEngineName = "3d_engine";
  appinfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  appinfo.apiVersion = VK_API_VERSION_1_3;

  auto exts = get_req_extentions();

  VkInstanceCreateInfo createinfo{};
  createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createinfo.pApplicationInfo = &appinfo;
  createinfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
  createinfo.ppEnabledExtensionNames = exts.data();

  if (ENABLE_VALIDATION_LAYERS) {
    createinfo.enabledLayerCount =
        static_cast<uint32_t>(validation_layers.size());
    createinfo.ppEnabledLayerNames = validation_layers.data();

    VkDebugUtilsMessengerCreateInfoEXT dbg_createinfo{};
    populate_dbg_messenger(dbg_createinfo);
    createinfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&dbg_createinfo;
  } else {
    createinfo.enabledLayerCount = 0;
    createinfo.pNext = nullptr;
  }

  VkResult result = vkCreateInstance(&createinfo, nullptr, &vk_instance);
  if (result != VK_SUCCESS) {
    LOG("Failed to create instance with result: %d\n", result);
    return VulkanError::CREATE_INSTANCE;
  }

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensions.data());

  std::string out = "extentions:";

  for (const auto &extension : extensions) {
    out += std::string("\n\t") + extension.extensionName;
  }
  LOG("%s\n", out.data());

  return std::nullopt;
}

uint32_t rate_device(VkPhysicalDevice dev) {
  VkPhysicalDeviceProperties prop;
  vkGetPhysicalDeviceProperties(dev, &prop);

  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceFeatures(dev, &features);

  if (!features.geometryShader) {
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

std::optional<VulkanError> pick_phisical_dev() {

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
    candidates.insert(std::make_pair(rate_device(dev), dev));
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

std::optional<VulkanError> init() {

  std::optional<VulkanError> ret = create_vk_instance();
  if (ret.has_value()) {
    return ret;
  }

  ret = setup_dbg_messenger();
  if (ret.has_value()) {
    return ret;
  }

  ret = pick_phisical_dev();
  if (ret.has_value()) {
    return ret;
  }

  // window = std::make_shared<Window>(Window());
  // window->init(1920, 1080, (char *)"3d_engine", false, true, true, true);
  // window->terminate();
  // return window;
  return std::nullopt;
}

void terminate() {

  // window->terminate();
  if (ENABLE_VALIDATION_LAYERS) {
    DestroyDebugUtilsMessengerEXT(vk_instance, dbg_messenger, nullptr);
  }

  vkDestroyInstance(vk_instance, nullptr);
}

// Validation Layer
bool check_vl_support() {
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> aval_layers(layer_count);

  vkEnumerateInstanceLayerProperties(&layer_count, aval_layers.data());

  for (const char *name : validation_layers) {
    bool found = false;

    for (const auto &prop : aval_layers) {
      if (strcmp(name, prop.layerName) == 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      LOG("%s not found\n", name);
      return false;
    }
  }

  return true;
}
} // namespace Vulkan
} // namespace En
