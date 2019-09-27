#pragma once

#include <vector>
#include <string>
#include "scomponents/graphics/materials.h"

namespace comp {
	/**
	 * @brief Vertex attribute buffer (points, uv mapping, normals, etc...)
	 */
	struct AttributeBuffer {
		unsigned int bufferId;
		unsigned int stride = 0;
		unsigned int count = 0;
		unsigned int byteWidth = 0;
	};

	/**
	 * @brief Keep references to vertex attributes sent to GPU
	 *
	 * @note Attributes are stored in separate buffers (PPP)(TTT)(NNN).
	 *		 This instead of interleaved (PTNPTNPTN) or packed (PPPTTTNNN).
	 */
	struct VertexBuffer {
		std::vector<unsigned int> bufferIds;
		std::vector<unsigned int> strides;
		std::vector<unsigned int> counts;
		std::vector<unsigned int> byteWidths;
		std::vector<unsigned int> offsets;
		std::vector<std::string> names;
		unsigned int vertexArrayId;
	};

	struct VertexArray {
		unsigned int bufferId;
	};

	/**
	 * @brief Data used to prevent vertex duplication
	 */
	struct IndexBuffer {
		unsigned int bufferId;
		unsigned int count = 0;
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
	};
}
