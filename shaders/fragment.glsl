#version 460 core
in VS_OUT{
  vec3 pos;
  vec3 normal;
  vec2 tex;
  vec4 color;
} fs_in;

out vec4 fs_color;
uniform sampler2D tex;

void main(){
  fs_color = texture(tex,fs_in.tex);
  if(fs_color.a == 0.0)
    discard;
};
