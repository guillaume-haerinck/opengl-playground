#version 300 es

layout(location = 0) in vec3 position;
layout(location = 1) in mat4 test;

layout (std140) uniform perMesh {
    lowp mat4 matModel;
    lowp uint materialIndex;
};

layout (std140) uniform perFrame {
    lowp mat4 matViewProj;
	lowp vec3 cameraPos;
};

void main() {
	gl_Position = matViewProj * test * vec4(position, 1.0);
}
