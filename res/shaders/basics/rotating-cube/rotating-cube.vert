#version 300 es
layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 position;

layout (std140) uniform perMesh {
    lowp mat4 matModel;
    lowp uint materialIndex;
};

layout (std140) uniform perFrame {
    lowp mat4 matViewProj;
	lowp vec3 cameraPos;
};

void main() {
	gl_Position = matViewProj * matModel * vec4(position, 1.0);
}
