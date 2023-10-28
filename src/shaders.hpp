#pragma once

const char *basic_vertex_shader =
    "#version 460 core\n"
    "layout (location = 0) in vec3 vert_pos;\n"
    "layout (location = 1) in vec4 vert_color;\n"
    "out vec4 frag_color;"
    "void main(){\n"
    "frag_color = vert_color;"
    "gl_Position = vec4(vert_pos.x,vert_pos.y,vert_pos.z,1.f);\n"
    "}\0";

const char *camera_vs =
    R"(#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texcoord;
layout(location = 3) in vec4 vertex_color;

out vec3 vs_position;
// out vec3 vs_normal;
out vec2 vs_texcoord;
out vec4 frag_color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() {
  // vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
  // vs_position = vertex_position;
  frag_color = vertex_color;
  vs_texcoord = vertex_texcoord;
  // vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
  // vs_normal = mat3(ModelMatrix) * vertex_normal;

  gl_Position =
      ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
      // ProjectionMatrix * ViewMatrix * vec4(vertex_position, 1.f);
}
)";

const char *basic_fs = "#version 460 core\n"
                       "in vec4 frag_color;\n"
                       "in vec2 vs_texcoord;\n"
                       "out vec4 FragColor;\n"
                       "uniform sampler2D tex;"
                       "void main(){\n"
                       // "FragColor = vec4(0.3f,1.f,0.5f,1.f);\n"
                       "FragColor = texture(tex,vs_texcoord)* frag_color;\n"
                       // "FragColor = frag_color;\n"
                       "}\0";
