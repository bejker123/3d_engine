#version 460 core
in vec4 _vs_color;
in vec2 _vs_texcoord;
out vec4 fs_color;
uniform sampler2D tex;

void main(){
  fs_color = texture(tex,_vs_texcoord);
  if(fs_color.a == 0.0)
    discard;
};
