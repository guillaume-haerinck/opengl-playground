#version 300 es
layout(location = 0) out lowp vec4 outColor;

layout (std140) uniform perCustomChanges {
    vec3 color;
};

void main() {
	outColor = vec4(color, 1);
}
