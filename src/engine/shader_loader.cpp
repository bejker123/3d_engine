#include "shader_loader.hpp"
#include "io/logger.hpp"
#include <fstream>
#include <string>
#include <utility>

namespace En {
std::map<uint32_t, std::tuple<const char *, const char *, const char *>> paths;
std::optional<ll::Shader> ShaderLoader::load(const char *vs_path,
                                             const char *fs_path,
                                             const char *gs_path) {
  LOG("[SHADER_LOADER] Loading:\n\tVertex: %s\n\tFragment: %s\n", vs_path,
      fs_path);
  if (strlen(gs_path))
    printf("\tGeometry: %s\n", gs_path);
  std::ifstream vs_file, fs_file, gs_file;
  vs_file.open(vs_path);
  if (!vs_file.is_open()) {
    LOG("Failed to load vs: %s\n", vs_path);
    return std::nullopt;
  }

  std::string line;
  std::string vs_content;

  while (std::getline(vs_file, line))
    vs_content += line + '\n';

  fs_file.open(fs_path);
  if (!fs_file.is_open()) {
    LOG("Failed to load fs: %s\n", fs_path);
    return std::nullopt;
  }

  std::string fs_content;

  while (std::getline(fs_file, line))
    fs_content += line + '\n';

  std::string gs_content = "";

  if (strlen(gs_path))
    gs_file.open(gs_path);
  if (!gs_file.is_open() && strlen(gs_path)) {
    LOG("Failed to load gs: %s\n", gs_path);
    // return std::nullopt;
  } else {

    while (std::getline(gs_file, line))
      gs_content += line + '\n';
  }
  auto ret =
      ll::Shader(vs_content.data(), fs_content.data(), gs_content.data());
  paths.insert(
      std::make_pair(ret.get_id(), std::tuple(vs_path, fs_path, gs_path)));
  return std::optional(ret);
}

std::optional<ll::Shader> ShaderLoader::reload(const uint32_t id) {
  if (!paths.contains(id))
    return std::nullopt;

  auto [vp, fp, gp] = paths.at(id);
  paths.erase(id);
  LOG("[SHADER_LOADER] Reloading %d:\n", id);

  return std::optional(ShaderLoader::load(vp, fp, gp));
}

} // namespace En
