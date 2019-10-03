#version 300 es
layout(location = 0) in vec2 position;

layout (std140) uniform perFrame {
    lowp mat4 matViewProj;
	lowp vec3 cameraPos;
};

void main() {
	gl_Position = vec4(position, 0.0, 1.0) * matViewProj;
}
