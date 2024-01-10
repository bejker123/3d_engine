#include "vulkan.hpp"
#include "../../io/logger.hpp"
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>
#include <vulkan/vulkan.h>

namespace En {

static std::shared_ptr<Vulkan> _instance;
static VkInstance vk_instance;
static std::shared_ptr<Window> window;

Vulkan::Vulkan() {}
Vulkan::~Vulkan() {}

std::optional<std::shared_ptr<Window>> Vulkan::init() {
  _instance = std::shared_ptr<Vulkan>(new Vulkan());
  VkApplicationInfo appinfo{};
  appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appinfo.pApplicationName = "3d_engine";
  appinfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  appinfo.pEngineName = "3d_engine";
  appinfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  appinfo.apiVersion = VK_API_VERSION_1_3;

  uint32_t glfw_ext_count = 0;
  const char **glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);

  VkInstanceCreateInfo createinfo{};
  createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createinfo.pApplicationInfo = &appinfo;
  createinfo.enabledExtensionCount = glfw_ext_count;
  createinfo.ppEnabledExtensionNames = glfw_exts;
  createinfo.enabledLayerCount = 0;

  VkResult result = vkCreateInstance(&createinfo, nullptr, &vk_instance);
  if (result != VK_SUCCESS) {
    LOG("[VULKAN] Failed to create instance with result: %d\n", result);
    return std::nullopt;
  }

  // std::vector<const char *> req_exts;
  //
  // for (size_t i = 0; i < glfw_ext_count; i++) {
  // }
  //

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         extensions.data());

  std::string out = "[VULKAN] extentions:";

  for (const auto &extension : extensions) {
    out += std::string("\n\t") + extension.extensionName;
  }
  LOG("%s\n", out.data());

  window = std::make_shared<Window>(Window());
  window->init(1920, 1080, (char *)"3d_engine", false, true, true, true);
  return window;
}

std::shared_ptr<Vulkan> Vulkan::instance() {
  assert(_instance != nullptr);
  return _instance;
}

void Vulkan::terminate() {
  vkDestroyInstance(vk_instance, nullptr);
  window->terminate();
}

} // namespace En
