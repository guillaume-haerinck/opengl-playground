#include "render-command.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <sstream>
#include <fstream>

#include "graphics/gl-exception.h"
#include "graphics/constant-buffer.h"

void deleteMeshBuffers(entt::entity entity, entt::registry & registry) {
	comp::Mesh& mesh = registry.get<comp::Mesh>(entity);
	GLCall(glDeleteVertexArrays(1, &mesh.vb.vertexArrayId));
	GLCall(glDeleteBuffers(1, &mesh.ib.bufferId));

	for (auto ab : mesh.vb.bufferIds) {
		GLCall(glDeleteBuffers(1, &ab));
	}
}

RenderCommand::RenderCommand(entt::registry& registry) : m_registry(registry)
{
	registry.on_destroy<comp::Mesh>().connect<&deleteMeshBuffers>();
}

RenderCommand::~RenderCommand() {
	scomp::Pipelines& pipelines = m_registry.ctx<scomp::Pipelines>();
	for (auto pipeline : pipelines.pipelines) {
		GLCall(glDeleteProgram(pipeline.programIndex));
	}

	scomp::ConstantBuffers& cbs = m_registry.ctx<scomp::ConstantBuffers>();
	for (auto cb : cbs.constantBuffers) {
		GLCall(glDeleteBuffers(1, &cb.bufferId));
	}
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

comp::VertexBuffer RenderCommand::createVertexBuffer(const VertexInputDescription& vib, comp::AttributeBuffer* attributeBuffers, unsigned int count) const {
	GLuint va;
	GLCall(glGenVertexArrays(1, &va));
	GLCall(glBindVertexArray(va));

	// Set layout
	unsigned int vbIndex = 0;
	for (const auto& element : vib) {
		GLCall(glEnableVertexAttribArray(vbIndex));
		GLCall(glVertexAttribPointer(
			vbIndex,
			element.getComponentCount(),
			shaderDataTypeToOpenGLBaseType(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			vib.getStride(),
			(const void*)(intptr_t)element.offset
		));
		vbIndex++;
	}

	// Set buffers
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
	scomp::ConstantBuffers& cbs = m_registry.ctx<scomp::ConstantBuffers>();
	cbs.constantBuffers.at(index) = cb;

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

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);
		glDeleteProgram(programId);

		spdlog::error("[createPipeline] Cannot link shader ! {}", infoLog.data());
		assert(false);
	}

	// Link constant buffers
	scomp::ConstantBuffers& cbs = m_registry.ctx<scomp::ConstantBuffers>();
	scomp::Pipeline sPipeline = {};
	for (size_t i = 0; i < cbCount; i++) {
		scomp::ConstantBuffer cb = cbs.constantBuffers.at(cbIndices[i]);
		unsigned int blockIndex = glGetUniformBlockIndex(programId, cb.name.c_str());
		GLCall(glUniformBlockBinding(programId, blockIndex, i));
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, i, cb.bufferId));
		sPipeline.constantBufferIndices.push_back(cbIndices[i]);
	}
	
	// Save to singleton components
	scomp::Pipelines& pipelines = m_registry.ctx<scomp::Pipelines>();
	sPipeline.shaders = shaders;
	sPipeline.programIndex = programId;
	pipelines.pipelines.push_back(sPipeline);

	// Return result
	comp::Pipeline pipeline = {};
	pipeline.index = pipelines.pipelines.size() - 1;
	return pipeline;
}

void RenderCommand::bindVertexBuffer(comp::VertexBuffer vb) const {
	GLCall(glBindVertexArray(vb.vertexArrayId));
}

void RenderCommand::bindIndexBuffer(comp::IndexBuffer ib) const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.bufferId));
}

void RenderCommand::bindTextures(unsigned int* texturesIds, unsigned int count) const {
}

void RenderCommand::bindPipeline(comp::Pipeline pipeline) const {
	scomp::Pipelines& pipelines = m_registry.ctx<scomp::Pipelines>();
	auto sPipeline = pipelines.pipelines.at(pipeline.index);
	GLCall(glUseProgram(sPipeline.programIndex));
}

void RenderCommand::updateConstantBuffer(scomp::ConstantBuffer cb, void* data) const {
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
