#pragma once
#include <GL/glew.h>

namespace En {

#define OPENGL_VER_MAJ 4
#define OPENGL_VER_MIN 6

namespace opengl {

bool setup_glew();

void debug_info();
void clear_buffer();

}; // namespace opengl

} // namespace En
