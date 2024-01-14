#pragma once
#include <optional>
#include <sstream>
#include <vector>
#define MAKE_ENUM(name, ...)                                                   \
  enum class name { __VA_ARGS__, __COUNT };                                    \
  inline std::ostream &operator<<(std::ostream &os, name value) {              \
    std::string enumName = #name;                                              \
    std::string str = #__VA_ARGS__;                                            \
    size_t len = str.length();                                                 \
    std::vector<std::string> strings;                                          \
    std::ostringstream temp;                                                   \
    for (size_t i = 0; i < len; i++) {                                         \
      if (isspace(str[i]))                                                     \
        continue;                                                              \
      else if (str[i] == ',') {                                                \
        strings.push_back(temp.str());                                         \
        temp.str(std::string());                                               \
      } else                                                                   \
        temp << str[i];                                                        \
    }                                                                          \
    strings.push_back(temp.str());                                             \
    os << enumName << "::" << strings[static_cast<size_t>(value)];             \
    return os;                                                                 \
  }

MAKE_ENUM(VulkanError, NO_VALIDATION_LAYERS, CREATE_INSTANCE,
          SETUP_DBG_MESSENGER, NO_VULKAN_DEVICE, NO_SUITABLE_DEVICE,
          CREATE_DEVICE, CREATE_SURFACE);

using VulkanErr = std::optional<VulkanError>;
