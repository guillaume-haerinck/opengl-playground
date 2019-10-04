#version 300 es
layout(location = 0) out lowp vec4 outColor;

layout (std140) uniform perMesh {
    lowp mat4 matModel;
    lowp uint materialIndex;
};

layout (std140) uniform perFrame {
    lowp mat4 matViewProj;
    lowp vec3 cameraPos;
};

void main() {
	outColor = vec4(cameraPos, 1);
}
