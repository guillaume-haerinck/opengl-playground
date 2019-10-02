#version 300 es
layout(location = 0) out highp vec4 outColor;

layout (std140) uniform perCustomChanges {
    lowp vec3 color;
};

void main() {
	outColor = vec4(color, 1.0f);
}
