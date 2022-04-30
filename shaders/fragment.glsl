#version 460 core

out vec4 fragColor;
in vec3 vtxColor;
in vec2 vtxTexCoord;

uniform sampler2D uniTex;

void main()
{
    fragColor = texture(uniTex, vtxTexCoord);
}