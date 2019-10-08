#version 300 es
layout(location = 0) out lowp vec4 outColor;

in lowp vec2 in_texCoord;

uniform sampler2D in_texture;

layout (std140) uniform perFrame {
    lowp mat4 matViewProj;
    lowp vec3 cameraPos;
};

void main() {
	outColor = texture(in_texture, in_texCoord);
}
