#include "render-command.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <sstream>
#include <fstream>
#include <stb_image/stb_image.h>

#include "graphics/gl-exception.h"
#include "graphics/constant-buffer.h"
#include "core/context.h"

void deleteMeshBuffers(entt::entity entity, entt::registry & registry) {
	comp::Mesh& mesh = registry.get<comp::Mesh>(entity);
	GLCall(glDeleteVertexArrays(1, &mesh.vb.vertexArrayId));
	GLCall(glDeleteBuffers(1, &mesh.ib.bufferId));

	for (auto ab : mesh.vb.bufferIds) {
		GLCall(glDeleteBuffers(1, &ab));
	}
}

RenderCommand::RenderCommand(Context& context) : m_ctx(context)
{
	m_ctx.registry.on_destroy<comp::Mesh>().connect<&deleteMeshBuffers>();
}

RenderCommand::~RenderCommand() {
	for (auto pipeline : m_ctx.pipelines) {
		GLCall(glDeleteProgram(pipeline.programIndex));
	}

	for (auto cb : m_ctx.constantBuffers) {
		GLCall(glDeleteBuffers(1, &cb.bufferId));
	}

	for (auto material : m_ctx.phongMaterials.materials) {
		for (auto texture : material.textures) {
			GLCall(glDeleteTextures(1, &texture.id));
		}
	}
}

void RenderCommand::clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

scomp::Texture RenderCommand::createTexture(unsigned int slot, const char* filepath) const {
	int width, height, bpp;
	stbi_set_flip_vertically_on_load(true); // Because 0,0 is bottom left in OpenGL
	unsigned char* localBuffer = stbi_load(filepath, &width, &height, &bpp, 4);
	if (!localBuffer) {
		spdlog::critical("[Texture] Unable to open texture {}", filepath);
		debug_break();
	}

	unsigned int id;
	GLCall(glGenTextures(1, &id));
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));

	// TODO remove and use sampler instead
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind

	if (localBuffer) {
		stbi_image_free(localBuffer);
	}

	// Return result
	scomp::Texture texture = {};
	texture.id = id;
	texture.samplerSlot = 0;
	texture.slot = slot;

	return texture;
}

comp::AttributeBuffer RenderCommand::createAttributeBuffer(const void* vertices, unsigned int count, unsigned int stride) const {
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

comp::VertexBuffer RenderCommand::createVertexBuffer(const VertexInputDescription& vib, comp::AttributeBuffer* attributeBuffers) const {
	GLuint va;
	GLCall(glGenVertexArrays(1, &va));
	GLCall(glBindVertexArray(va));

	// Set layout
	unsigned int vbIndex = 0;
	for (const auto& element : vib) {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[vbIndex].bufferId));
		GLCall(glEnableVertexAttribArray(vbIndex));
		GLCall(glVertexAttribPointer(
			vbIndex,
			element.getComponentCount(),
			shaderDataTypeToOpenGLBaseType(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			element.size,
			(const void*)(intptr_t)element.offset
		));
		vbIndex++;
	}

	GLCall(glBindVertexArray(0));

	comp::VertexBuffer vb = {};
	vb.vertexArrayId = va;
	return vb;
}

comp::IndexBuffer RenderCommand::createIndexBuffer(const void* indices, unsigned int count) const {
	unsigned int id;
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	comp::IndexBuffer buffer = {};
	buffer.bufferId = id;
	buffer.count = count;

	return buffer;
}

scomp::ConstantBuffer RenderCommand::createConstantBuffer(scomp::ConstantBufferIndex index, unsigned int byteWidth, void* data) const {
	std::string name = "";
	switch (index) {
		case scomp::PER_MESH: name = "perMesh"; break;
		case scomp::PER_MESH_BATCH: name = "perMeshBatch"; break;
		case scomp::PER_FRAME: name = "perFrame"; break;
		case scomp::PER_PHONG_MAT_CHANGE: name = "perPhongMatChange"; break;
		case scomp::PER_COOK_MAT_CHANGE: name = "perCookMatChange"; break;
		case scomp::PER_LIGHT_CHANGE: name = "perLightChange"; break;
		case scomp::PER_CUSTOM_PROP_CHANGE_0: name = "perCustomPropChange0"; break;
		case scomp::PER_CUSTOM_PROP_CHANGE_1: name = "perCustomPropChange1"; break;
		case scomp::PER_CUSTOM_PROP_CHANGE_2: name = "perCustomPropChange2"; break;
		default:
			spdlog::error("[createConstantBuffer] unknown index {}", index);
		break;
	}
	
	unsigned int cbId = 0;
	GLCall(glGenBuffers(1, &cbId));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, cbId));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, byteWidth, data, GL_DYNAMIC_COPY));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	scomp::ConstantBuffer cb = {};
	cb.bufferId = cbId;
	cb.byteWidth = byteWidth; // TODO assert that it is a multiple of 16
	cb.name = name;

	// Save to singleton components
	m_ctx.constantBuffers.at(index) = cb;

	return cb;
}

scomp::VertexShader RenderCommand::createVertexShader(const char* filePath) const {
	// Compile shader
	std::string shader = readTextFile(filePath);
	const char* src = shader.c_str();
	unsigned int vsId = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vsId, 1, &src, nullptr));
	GLCall(glCompileShader(vsId));
	if (!hasShaderCompiled(vsId, GL_VERTEX_SHADER)) {
		spdlog::info("[Shader] {}", filePath);
		spdlog::info("\n{}", shader);
		debug_break();
	}

	// Return result
	scomp::VertexShader vs = {};
	vs.shaderId = vsId;
	return vs;
}

scomp::FragmentShader RenderCommand::createFragmentShader(const char* filePath) const {
	// Compile shader
	std::string shader = readTextFile(filePath);
	const char* src = shader.c_str();
	unsigned int fsId = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(fsId, 1, &src, nullptr));
	GLCall(glCompileShader(fsId));
	if (!hasShaderCompiled(fsId, GL_FRAGMENT_SHADER)) {
		spdlog::info("[Shader] {}", filePath);
		spdlog::info("\n{}", shader);
		debug_break();
	}

	// Return result
	scomp::FragmentShader fs = {};
	fs.shaderId = fsId;

	return fs;
}

comp::Pipeline RenderCommand::createPipeline(const scomp::ShaderPipeline& shaders, scomp::ConstantBufferIndex* cbIndices, unsigned int cbCount) const {
	// TODO generate hash and check hashmap to see if pipeline already exist

	// Compile pipeline
	unsigned int programId = glCreateProgram();
	if (shaders.hasShader.at(scomp::PipelineShaderIndex::VS)) { GLCall(glAttachShader(programId, shaders.vs.shaderId)); }
	if (shaders.hasShader.at(scomp::PipelineShaderIndex::FS)) { GLCall(glAttachShader(programId, shaders.fs.shaderId)); }

	GLCall(glLinkProgram(programId));

	if (shaders.hasShader.at(scomp::PipelineShaderIndex::VS)) { GLCall(glDeleteShader(shaders.vs.shaderId)); }
	if (shaders.hasShader.at(scomp::PipelineShaderIndex::FS)) { GLCall(glDeleteShader(shaders.fs.shaderId)); }
	
	// Check errors
	GLCall(glValidateProgram(programId));
	GLint isLinked = 0;
	GLCall(glGetProgramiv(programId, GL_LINK_STATUS, (int*)&isLinked));
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		// FIMXE
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);
		glDeleteProgram(programId);

		spdlog::error("[createPipeline] Cannot link shader !");
		debug_break();
		assert(false);
	}

	// Link constant buffers
	scomp::Pipeline sPipeline = {};
	for (size_t i = 0; i < cbCount; i++) {
		scomp::ConstantBuffer& cb = m_ctx.constantBuffers.at(cbIndices[i]);
		unsigned int blockIndex = glGetUniformBlockIndex(programId, cb.name.c_str());
		GLCall(glUniformBlockBinding(programId, blockIndex, i));
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, i, cb.bufferId));
		sPipeline.constantBufferIndices.push_back(cbIndices[i]);
	}
	
	// Save to singleton components
	sPipeline.shaders = shaders;
	sPipeline.programIndex = programId;
	m_ctx.pipelines.push_back(sPipeline);

	// Return result
	comp::Pipeline pipeline = {};
	pipeline.index = m_ctx.pipelines.size() - 1;
	return pipeline;
}

void RenderCommand::bindVertexBuffer(const comp::VertexBuffer& vb) const {
	GLCall(glBindVertexArray(vb.vertexArrayId));
}

void RenderCommand::bindIndexBuffer(const comp::IndexBuffer& ib) const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.bufferId));
}

void RenderCommand::bindTexture(const scomp::Texture& texture) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture.slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture.id));
}

void RenderCommand::bindPipeline(const comp::Pipeline& pipeline) const {
	scomp::Pipeline sPipeline = m_ctx.pipelines.at(pipeline.index);
	GLCall(glUseProgram(sPipeline.programIndex));
}

void RenderCommand::updateConstantBuffer(const scomp::ConstantBuffer& cb, void* data) const {
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, cb.bufferId));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, cb.byteWidth, data));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
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
			case GL_VERTEX_SHADER: return "vertex";
			case GL_FRAGMENT_SHADER: return "fragment";
			default: return "unknown type";
			}
		}();

		spdlog::error("[Shader] Failed to compile {} shader", typeString);
		spdlog::error("[Shader] {}", message);
		GLCall(glDeleteShader(shaderId));
		return false;
	}

	return true;
}

GLenum RenderCommand::shaderDataTypeToOpenGLBaseType(ShaderDataType type) const {
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

std::string RenderCommand::readTextFile(const char* filePath) const {
	std::ifstream fileStream(filePath);
	if (!fileStream) {
		spdlog::error("[readTextFile] Cannot load file : {}", filePath);
		debug_break();
		assert(false);
	}

	std::stringstream shaderStream;
	shaderStream << fileStream.rdbuf();
	fileStream.close();
	return shaderStream.str();
}
