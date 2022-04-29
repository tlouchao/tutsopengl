#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform vec4 uniColor;
out vec3 vtxColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vtxColor = aColor;
}