#include "render-command.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <stdio.h>

#include "graphics/gl-exception.h"

RenderCommand::RenderCommand()
{
}

RenderCommand::~RenderCommand()
{
}

void RenderCommand::clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
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

comp::IndexBuffer RenderCommand::createIndexBuffer(void* indices, unsigned int count) const {
	unsigned int id;
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	comp::IndexBuffer buffer = {};
	buffer.bufferId = id;
	buffer.count = count;

	return buffer;
}


scomp::VertexShader RenderCommand::createVertexShader(const char* filePath, const VertexInputDescription& vib) const {
	const char* vsSource = readShaderFile(filePath);

	// Compile shader
	unsigned int vsId = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vsId, 1, &vsSource, nullptr));
	GLCall(glCompileShader(vsId));
	hasShaderCompiled(vsId, GL_VERTEX_SHADER);

	// Assign vertex input description to vao
	unsigned int vbIndex = 0;
	GLCall(glBindVertexArray(vib.vertexArrayId));
	for (const auto& element : vib) {
		GLCall(glEnableVertexAttribArray(vbIndex));
		GLCall(glVertexAttribPointer(
			vbIndex,
			element.GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			vib.GetStride(),
			(const void*)(intptr_t)element.Offset
		));
		vbIndex++;
	}
	GLCall(glBindVertexArray(0));

	// Return result
	scomp::VertexShader vs = {};
	vs.shaderId = vsId;
	vs.inputDescription = vib;

	return vs;
}

scomp::FragmentShader RenderCommand::createFragmentShader(const char* filePath) const {
	const char* fragSource = readShaderFile(filePath);

	// Compile shader
	unsigned int fsId = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(fsId, 1, &fragSource, nullptr));
	GLCall(glCompileShader(fsId));
	hasShaderCompiled(fsId, GL_FRAGMENT_SHADER);

	// Return result
	scomp::FragmentShader fs = {};
	fs.shaderId = fsId;

	return fs;
}

comp::Pipeline RenderCommand::createPipeline(scomp::VertexShader vs, scomp::FragmentShader fs) const {
	// Compile pipeline
	unsigned int programId = glCreateProgram();
	GLCall(glAttachShader(programId, vs.shaderId));
	GLCall(glAttachShader(programId, fs.shaderId));
	GLCall(glLinkProgram(programId));
	
	// Check errors
	GLCall(glValidateProgram(programId));
	// TODO https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Platform/OpenGL/OpenGLShader.cpp

	// Return result
	comp::Pipeline pipeline = {};
	pipeline.vsIndex = vs.shaderId;
	pipeline.fsIndex = fs.shaderId;
	pipeline.index = programId;

	return pipeline;
}

void RenderCommand::bindVertexBuffer(comp::VertexBuffer vb) const {
	GLCall(glBindVertexArray(vb.vertexArrayId));
}

void RenderCommand::bindIndexBuffer(comp::IndexBuffer ib) const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.bufferId));
}

void RenderCommand::bindTextures(unsigned int* texturesIds, unsigned int count) const
{
}

void RenderCommand::bindPipeline(comp::Pipeline pipeline) const {
	GLCall(glUseProgram(pipeline.index));
}

void RenderCommand::updateConstantBuffer(scomp::ConstantBuffer cb, void* data) const
{
}

void RenderCommand::draw(unsigned int count) const {
	GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}

void RenderCommand::drawIndexed(unsigned int count) const {
	GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*) 0));
}

bool RenderCommand::hasShaderCompiled(unsigned int shaderId, unsigned int shaderType) const {
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderId, length, &length, message);
		auto const typeString = [shaderType]() {
			switch (shaderType) {
			case GL_VERTEX_SHADER: return "fragment";
			case GL_FRAGMENT_SHADER: return "vertex";
			default: return "unknown type";
			}
		}();

		spdlog::error("[Shader] Failed to compile {} shader", typeString);
		spdlog::error("[Shader] {}", message);
		debug_break();

		GLCall(glDeleteShader(shaderId));
		return false;
	}

	return true;
}

GLenum RenderCommand::ShaderDataTypeToOpenGLBaseType(ShaderDataType type) const {
	switch (type) {
	case ShaderDataType::Float:    return GL_FLOAT;
	case ShaderDataType::Float2:   return GL_FLOAT;
	case ShaderDataType::Float3:   return GL_FLOAT;
	case ShaderDataType::Float4:   return GL_FLOAT;
	case ShaderDataType::Mat3:     return GL_FLOAT;
	case ShaderDataType::Mat4:     return GL_FLOAT;
	case ShaderDataType::Int:      return GL_INT;
	case ShaderDataType::Int2:     return GL_INT;
	case ShaderDataType::Int3:     return GL_INT;
	case ShaderDataType::Int4:     return GL_INT;
	case ShaderDataType::Bool:     return GL_BOOL;
	}

	assert(false && "Unknown ShaderDataType!");
	return 0;
}

const char* RenderCommand::readShaderFile(const char* filePath) const {
	FILE* file = fopen(filePath, "rb");
	if (!file) {
		spdlog::error("[readShaderFile] Cannot load file : {}", filePath);
		debug_break();
		assert(false);
	}

	const unsigned int MAX_SHADER_SIZE = 1000; // Prevent dynamic allocation
	char shaderSrc[MAX_SHADER_SIZE];
	size_t count = fread(shaderSrc, 1, MAX_SHADER_SIZE - 1, file);

    assert(count < MAX_SHADER_SIZE - 1); // File was too long

    shaderSrc[count] = '\0';
    fclose(file);

	return shaderSrc;
}
