#pragma once

#include "components/graphics/mesh.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/shaders.h"

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
	scomp::ConstantBuffer createConstantBuffer(unsigned int byteWidth) const;

    /**
	 * @param iedArray - Input layout of the shader
	 * @param iedElementCount - Number of elements inside of the iedArray
	 * @param filepath - The relative path from the .exe to the .cso containing the shader
	 */
	scomp::VertexShader createVertexShader(unsigned char* filePath) const;

	/**
	 * @param filePath - The relative path from the .exe to the .cso containing the shader
	 */
	scomp::PixelShader createPixelShader(unsigned char* filePath) const;

    ///////////////////////////////////////////////////////////////////////////
	////////////////////////////////// BINDING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void bindVertexBuffer(comp::VertexBuffer vb) const;
	void bindIndexBuffer(comp::IndexBuffer ib) const;

	void bindTextures(unsigned int* texturesIds, unsigned int count) const;

	void bindVertexShader(scomp::VertexShader vs);
	void bindPixelShader(scomp::PixelShader ps);

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UPDATING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void updateConstantBuffer(scomp::ConstantBuffer cb, void* data) const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DRAWING /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void draw(unsigned int count) const;
	void drawIndexed(unsigned int count) const;
};
