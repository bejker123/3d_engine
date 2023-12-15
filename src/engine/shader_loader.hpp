#pragma once
#include "rendering/ll/shader.hpp"
#include <map>
#include <optional>

namespace En {
class ShaderLoader {
public:
  static std::optional<ll::Shader>
  load(const char *vs_path, const char *fs_path, const char *gs_path = "");

  static std::optional<ll::Shader> reload(const uint32_t id);

private:
};
} // namespace En
