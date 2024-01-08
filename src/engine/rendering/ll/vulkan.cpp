#include "vulkan.hpp"
#include "../../io/logger.hpp"
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>

namespace En {
bool Vulkan::init() {
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
    return false;
  }

  // std::vector<const char *> req_exts;
  //
  // for (size_t i = 0; i < glfw_ext_count; i++) {
  // }

  return true;
}

std::shared_ptr<Vulkan> Vulkan::instance() {
  assert(_instance != nullptr);
  return _instance;
}

void Vulkan::terminate() { vkDestroyInstance(vk_instance, nullptr); }

} // namespace En
