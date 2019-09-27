#pragma once

#include <glad/glad.h>

#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/shaders.h"
#include "graphics/vertex-input-description.h"

class RenderCommand {
public:
    RenderCommand();
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
	 * @param attributeBuffers - Array of buffers describing positions, normals, etc.
	 * @param count - The number of attribute buffers
	 */
	comp::VertexBuffer createVertexBuffer(comp::AttributeBuffer* attributeBuffers, unsigned int count) const;

    /**
	 * @param indices - Array of data
	 * @param count - The number of elements in the array
	 */
	comp::IndexBuffer createIndexBuffer(void* indices, unsigned int count) const;

    /**
	 * @param filepath - The relative path from the .exe to the .cso containing the shader
	 * @param ied - The input layout, do not forget to provide the vao from the vertex buffer
	 */
	scomp::VertexShader createVertexShader(const char* filePath, const VertexInputDescription& vib) const;

	/**
	 * @param filePath - The relative path from the .exe to the .cso containing the shader
	 */
	scomp::FragmentShader createFragmentShader(const char* filePath) const;

	/**
	 * 
	 */
	comp::Pipeline createPipeline(scomp::VertexShader vs, scomp::FragmentShader fs) const;

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

    void draw(unsigned int count) const;
	void drawIndexed(unsigned int count) const;

private:
	bool hasShaderCompiled(unsigned int shaderId, unsigned int shaderType) const;
	GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) const;
};
