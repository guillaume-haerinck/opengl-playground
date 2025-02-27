#pragma once

#include <glad/glad.h>
#include <entt/entt.hpp>
#include <string>

#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/pipelines.h"
#include "graphics/vertex-input-description.h"

struct Context; // Forward declaration to prevent circular inclusion

class RenderCommand {
public:
    RenderCommand(Context& context);
    ~RenderCommand();

    /**
	 * @brief Clear the back buffer
	 */
	void clear() const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// CREATION ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/**
	 * @note Must be added to a material, it will create a memory leak if it is not
	 * 
	 * @param slot - texture slot that the texture will be bound to
	 * @param filepath
	 */
	scomp::Texture createTexture(unsigned int slot, const char* filepath) const;

	/**
	 * @param vertices - Array of data
	 * @param count - The number of elements in the array
	 * @param stride - Size in bytes of one element of the array
	 * @param usage - (Optional) Used to know if the buffer is going to be updated or not
	 * @param type - (Optional) Used by instanced rendering to know which buffer to update
	 */
	comp::AttributeBuffer createAttributeBuffer(const void* vertices, unsigned int count, unsigned int stride, comp::AttributeBufferUsage usage = comp::AttributeBufferUsage::STATIC_DRAW, comp::AttributeBufferType type = comp::AttributeBufferType::PER_VERTEX_ANY) const;

	/**
	 * @param vib - Layout of the buffers
	 * @param attributeBuffers - Array of buffers describing positions, normals, etc. The count is defined by the vib size.
	 */
	comp::VertexBuffer createVertexBuffer(const VertexInputDescription& vib, comp::AttributeBuffer* attributeBuffers) const;

    /**
	 * @param indices - Array of data
	 * @param count - The number of elements in the array
	 */
	comp::IndexBuffer createIndexBuffer(const void* indices, unsigned int count, comp::IndexBuffer::dataType type) const;

	/**
	 * @param index 
	 * @note Save as singleton component
	 */
	scomp::ConstantBuffer createConstantBuffer(scomp::ConstantBufferIndex index, unsigned int byteWidth, void* data = nullptr) const;

    /**
	 * @param filepath - The relative path from the .exe to the .cso containing the shader
	 */
	scomp::VertexShader createVertexShader(const char* filePath) const;

	/**
	 * @param filePath - The relative path from the .exe to the .cso containing the shader
	 */
	scomp::FragmentShader createFragmentShader(const char* filePath) const;

	/**
	 *
	 */
	comp::Pipeline createPipeline(const scomp::ShaderPipeline& shaders, scomp::ConstantBufferIndex* cbIndices = nullptr, unsigned int cbCount = 0) const;

    ///////////////////////////////////////////////////////////////////////////
	////////////////////////////////// BINDING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void bindVertexBuffer(const comp::VertexBuffer& vb) const;
	void bindIndexBuffer(const comp::IndexBuffer& ib) const;

	void bindTexture(const scomp::Texture& texture) const;

	/**
	 * @brief Will bind all the shaders of the said pipeline
	 */
	void bindPipeline(const comp::Pipeline& pipeline) const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UPDATING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void updateConstantBuffer(const scomp::ConstantBuffer& cb, void* data) const;

	void updateAttributeBuffer(const comp::AttributeBuffer& buffer, void* data, unsigned int dataByteWidth) const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DRAWING /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void drawIndexed(unsigned int count, comp::IndexBuffer::dataType type) const;
	void drawIndexedInstances(unsigned int indexCount, comp::IndexBuffer::dataType type, unsigned int drawCount) const;

private:
	bool hasShaderCompiled(unsigned int shaderId, unsigned int shaderType) const;
	GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type) const;
	std::string readTextFile(const char* filePath) const;
	GLenum indexBufferDataTypeToOpenGLBaseType(comp::IndexBuffer::dataType) const;

private:
	Context& m_ctx;
};

// Callback function on Mesh component destruction
void deleteMeshBuffers(entt::entity entity, entt::registry & registry);
