#version 300 es

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 position;
layout(location = 3) in vec2 test;

out lowp vec2 v_test;

layout (std140) uniform perMesh {
    lowp mat4 matModel;
    lowp uint materialIndex;
};

layout (std140) uniform perFrame {
    lowp mat4 matViewProj;
	lowp vec3 cameraPos;
};

void main() {
	v_test = test;
	gl_Position = matViewProj * matModel * vec4(position.x, position.y, test.y, 1.0);
}
