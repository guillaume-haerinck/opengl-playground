#pragma once

#include <vector>

#include "graphics/vertex-input-description.h"

namespace scomp {

	/**
	 * @brief A vertex shader. The first stage in the graphic pipeline.
	 *
	 * @note Constant buffers will be bound but not updated by this component.
	 *		 The slot will correspond to the index of the vector the constant buffer is in.
	 */
	struct VertexShader {
		unsigned int shaderId;
		std::vector<unsigned int> constantBufferIds;
		VertexInputDescription inputDescription;
	};

	/**
	 * @brief A pixel shader. The last stage in the graphic pipeline.
	 *
	 * @note Constant buffers will be bound but not updated by this component.
	 *		 The slot will correspond to the index of the vector the constant buffer is in.
	 */
	struct FragmentShader {
		unsigned int shaderId;
		std::vector<unsigned int> constantBufferIds;
	};

	/**
	 * @brief 
	 */
	struct Shaders {
		std::vector<VertexShader> vss;
		std::vector<FragmentShader> pss;
	};
}
