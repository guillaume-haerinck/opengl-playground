#pragma once

#include <glad/glad.h>
#include <entt/entt.hpp>

#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/pipelines.h"
#include "graphics/vertex-input-description.h"

class RenderCommand {
public:
    RenderCommand(entt::registry& registry, entt::entity graphicEntity);
    ~RenderCommand();

    /**
	 * @brief Clear the back buffer
	 */
	void clear() const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// CREATION ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/**
	 * @param vertices - Array of data
	 * @param count - The number of elements in the array
	 * @param stride - Size in bytes of one element of the array
	 */
	comp::AttributeBuffer createAttributeBuffer(void* vertices, unsigned int count, unsigned int stride) const;

	/**
	 * @param vib - Layout of the buffers
	 * @param attributeBuffers - Array of buffers describing positions, normals, etc.
	 * @param count - The number of attribute buffers
	 */
	comp::VertexBuffer createVertexBuffer(const VertexInputDescription& vib, comp::AttributeBuffer* attributeBuffers, unsigned int count) const;

    /**
	 * @param indices - Array of data
	 * @param count - The number of elements in the array
	 */
	comp::IndexBuffer createIndexBuffer(void* indices, unsigned int count) const;

	/**
	 * @param byteWidth - The total size in bytes of the buffer (must be a multiple of 16)
	 *
	 * @note - You have to store it within a shader component, and it will be bound with it.
	 *		   The slot will correspond to the index of the vector in the shader.
	 */
	scomp::ConstantBuffer createConstantBuffer(unsigned int byteWidth, const char* name) const;

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
	// TODO bind cb to pipeline, send an array ?
	comp::Pipeline createPipeline(scomp::VertexShader vs, scomp::FragmentShader fs, scomp::ConstantBufferIndex* cbIndices = nullptr, unsigned int cbCount = 0) const;



    ///////////////////////////////////////////////////////////////////////////
	////////////////////////////////// BINDING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void bindVertexBuffer(comp::VertexBuffer vb) const;
	void bindIndexBuffer(comp::IndexBuffer ib) const;

	void bindTextures(unsigned int* texturesIds, unsigned int count) const;

	/**
	 * @brief Will bind all the shaders of the said pipeline
	 */
	void bindPipeline(comp::Pipeline pipeline) const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UPDATING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void updateConstantBuffer(scomp::ConstantBuffer cb, void* data) const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DRAWING /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void drawIndexed(unsigned int count) const;
	void drawIndexedInstanced(unsigned int indexCount, unsigned int drawCount) const;

private:
	bool hasShaderCompiled(unsigned int shaderId, unsigned int shaderType) const;
	GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type) const;

private:
	entt::registry& m_registry;
	entt::entity m_graphicEntity;
};


// Callback function on Mesh component destruction
void deleteMeshBuffers(entt::entity entity, entt::registry & registry);
