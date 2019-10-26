#version 300 es

layout(location = 0) out lowp vec4 outColor;

in lowp vec2 v_test;

layout (std140) uniform perFrame {
    lowp mat4 matViewProj;
    lowp vec3 cameraPos;
};

void main() {
	outColor = vec4(v_test, 1.0f, 1.0f);
}
