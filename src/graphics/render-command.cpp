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

comp::IndexBuffer RenderCommand::createIndexBuffer(void* indices, unsigned int count) const
{
	return comp::IndexBuffer();
}

comp::VertexArray RenderCommand::createVertexArray(comp::VertexBuffer* vbArray, unsigned int vbCount) const {
	return comp::VertexArray();
}

scomp::ConstantBuffer RenderCommand::createConstantBuffer(unsigned int byteWidth) const
{
	return scomp::ConstantBuffer();
}

scomp::VertexShader RenderCommand::createVertexShader(unsigned char* filePath) const
{
	return scomp::VertexShader();
}

scomp::PixelShader RenderCommand::createPixelShader(unsigned char* filePath) const
{
	return scomp::PixelShader();
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

void RenderCommand::bindVertexShader(scomp::VertexShader vs)
{
}

void RenderCommand::bindPixelShader(scomp::PixelShader ps)
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
