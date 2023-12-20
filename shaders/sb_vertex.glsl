#version 460 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 vs_normal;
layout(location = 2) in vec2 vs_texcoord;
layout(location = 3) in vec4 vs_color;

out vec3 TexCoords;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
    TexCoords = aPos;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(aPos, 1.0);
} 
