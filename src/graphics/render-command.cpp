#include "render-command.h"

#include <glad/glad.h>
#include "graphics/gl-exception.h"

RenderCommand::RenderCommand()
{
}

RenderCommand::~RenderCommand()
{
}

void RenderCommand::clear() const {
}

comp::AttributeBuffer RenderCommand::createAttributeBuffer(void* vertices, unsigned int count, unsigned int stride) const {
	unsigned int id;
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, stride * count, vertices, GL_STATIC_DRAW));

	comp::AttributeBuffer buffer = {};
	buffer.bufferId = id;
	buffer.byteWidth = stride * count;
	buffer.count = count;
	buffer.stride = stride;

	return buffer;
}

comp::VertexBuffer RenderCommand::createVertexBuffer(comp::AttributeBuffer* attributeBuffers, unsigned int count) const {
	GLuint va;
	GLCall(glGenVertexArrays(1, &va));
	GLCall(glBindVertexArray(va));

	for (size_t i = 0; i < count; i++) {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[i].bufferId));
	}

	GLCall(glBindVertexArray(0));

	comp::VertexBuffer vb = {};
	vb.vertexArrayId = va;
		
	return vb;
}

comp::IndexBuffer RenderCommand::createIndexBuffer(void* indices, unsigned int count) const
{
	return comp::IndexBuffer();
}


scomp::VertexShader RenderCommand::createVertexShader(const char* filePath, VertexInputDescription vib) const
{
	const char* vsSource = R"(#version 300 es
			layout(location = 0) in vec2 position;

			void main() {
				gl_Position = vec4(position, 0.0, 1.0);
			}
	)";

	return scomp::VertexShader();
}

scomp::FragmentShader RenderCommand::createFragmentShader(const char* filePath) const
{
	const char* fragSource = R"(#version 300 es
			layout(location = 0) out lowp vec4 color;

			void main() {
				color = vec4(1, 0, 0, 1);
			}
	)";

	return scomp::FragmentShader();
}

comp::Pipeline RenderCommand::createPipeline(scomp::VertexShader vs, scomp::FragmentShader ps) const
{
	return comp::Pipeline();
}

void RenderCommand::bindVertexBuffer(comp::VertexBuffer vb) const
{
}

void RenderCommand::bindIndexBuffer(comp::IndexBuffer ib) const
{
}

void RenderCommand::bindTextures(unsigned int* texturesIds, unsigned int count) const
{
}

void RenderCommand::bindPipeline(comp::Pipeline pipeline) const
{
}

void RenderCommand::updateConstantBuffer(scomp::ConstantBuffer cb, void* data) const
{
}

void RenderCommand::draw(unsigned int count) const
{
}

void RenderCommand::drawIndexed(unsigned int count) const
{
}
