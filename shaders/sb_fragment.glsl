#version 460 core
out vec4 FragColor;

in VS_OUT{
  // vec3 pos;
  // vec3 normal;
  vec3 tex;
  // vec4 color;
} fs_in;

uniform samplerCube tex;

void main()
{    
    FragColor = texture(tex, fs_in.tex);
}
