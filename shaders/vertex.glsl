#version 460
layout(location = 0) in vec3 vs_position;
layout(location = 1) in vec3 vs_normal;
layout(location = 2) in vec2 vs_texcoord;
layout(location = 3) in vec4 vs_color;

out vec3 _vs_position;
out vec2 _vs_texcoord;
out vec4 _vs_color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() {
  _vs_color = vs_color;
  _vs_texcoord = vs_texcoord;

  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vs_position, 1.f);
}
