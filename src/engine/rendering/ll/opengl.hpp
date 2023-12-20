#pragma once
#include <optional>
#include <string>

namespace En {

#define OPENGL_VER_MAJ 4
#define OPENGL_VER_MIN 6
#define OPENGL_DEBUG_INFO 1

namespace opengl {

bool setup();
bool setup_glew();

void debug_info();
void clear_buffer();
std::optional<std::string> get_error();

void terminate();

}; // namespace opengl

} // namespace En
