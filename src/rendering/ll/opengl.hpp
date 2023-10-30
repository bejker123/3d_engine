#ifndef OPENGL_HPP
#define OPENGL_HPP
#include <optional>
#include <string>

#define OPENGL_VER_MAJ 4
#define OPENGL_VER_MIN 6

namespace opengl {

bool setup();
bool setup_glew();

void debug_info();
void clear_buffer();
std::optional<std::string> get_error();

void terminate();

}; // namespace opengl

#endif // !OPENGL_HPP
