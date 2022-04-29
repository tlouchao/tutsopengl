#version 460 core

out vec4 fragColor;
in vec3 vtxColor;

void main()
{
    fragColor = vec4(vtxColor, 1.0);
}