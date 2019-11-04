#pragma once

#include <vector>
#include <string>
#include "scomponents/graphics/materials.h"

namespace comp {
	enum class AttributeBufferType {
		PER_VERTEX_ANY = 0,
		PER_INSTANCE_MODEL_MAT
	};

	enum class AttributeBufferUsage {
		STATIC_DRAW = 0,
		DYNAMIC_DRAW
	};

	/**
	 * @brief Vertex attribute buffer (points, uv mapping, normals, etc...)
	 */
	struct AttributeBuffer {
		unsigned int bufferId;
		unsigned int stride = 0;
		unsigned int count = 0;
		unsigned int byteWidth = 0;
		AttributeBufferUsage usage = AttributeBufferUsage::STATIC_DRAW;
		AttributeBufferType type = AttributeBufferType::PER_VERTEX_ANY;
	};

	/**
	 * @brief Keep references to vertex attributes sent to GPU
	 *
	 * @note Attributes are stored in separate buffers (PPP)(TTT)(NNN).
	 *		 This instead of interleaved (PTNPTNPTN) or packed (PPPTTTNNN).
	 */
	struct VertexBuffer {
		std::vector<AttributeBuffer> buffers;
		unsigned int vertexArrayId;
	};

	/**
	 * @brief Data used to prevent vertex duplication
	 */
	struct IndexBuffer {
		unsigned int bufferId;
		unsigned int count = 0;
	
		enum class dataType {
			UNSIGNED_BYTE, 
			UNSIGNED_SHORT,
			UNSIGNED_INT,
			NONE
		};
		dataType type = dataType::NONE;
	};

	/**
	 * @param materialIndex - The index in the std::vector of Materials singleton component (0 if default material).
	 *						  The same material can be shared by multiple mesh.
	 */
	struct Mesh {
		VertexBuffer vb;
		IndexBuffer ib;
		unsigned int materialIndex = 0;
		scomp::MaterialType materialType = scomp::MaterialType::NO_MATERIAL;
		bool isInstanced = false; // If true use batch rendering. Need a special attribute buffer to store instance data.
	};
}
