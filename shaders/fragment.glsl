#version 460 core

out vec4 fragColor;
in vec3 vtxColor;
in vec2 vtxTexCoord;

uniform sampler2D uniTex1;
uniform sampler2D uniTex2;

const vec4 oneVec = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
    //fragColor = texture(uniTex, vtxTexCoord);
    fragColor = mix(texture(uniTex1, vtxTexCoord), texture(uniTex2, vtxTexCoord), texture(uniTex2, vtxTexCoord).a * .8);
}