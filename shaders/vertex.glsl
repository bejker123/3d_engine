#version 460
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex;
layout(location = 3) in vec4 in_color;

out VS_OUT{
  vec3 pos;
  vec3 normal;
  vec2 tex;
  vec4 color;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
  vs_out.pos = in_pos;
  vs_out.normal = in_normal;
  vs_out.tex = in_tex;
  vs_out.color = in_color;

  gl_Position = proj * view * model * vec4(in_pos, 1.f);
}
