#pragma once

#include <vector>

namespace scomp {
	/**
	 * @brief A vertex shader. The first stage in the graphic pipeline.
	 *
	 * @note Constant buffers will be bound but not updated by this component.
	 *		 The slot will correspond to the index of the vector the constant buffer is in.
	 */
	struct VertexShader {
		unsigned int shaderId;
		unsigned int layoutId;
		std::vector<unsigned int> constantBufferIds;
	};

	/**
	 * @brief A pixel shader. The last stage in the graphic pipeline.
	 *
	 * @note Constant buffers will be bound but not updated by this component.
	 *		 The slot will correspond to the index of the vector the constant buffer is in.
	 */
	struct PixelShader {
		unsigned int shaderId;
		std::vector<unsigned int> constantBufferIds;
	};

	/**
	 * @brief 
	 */
	struct Shaders {
		std::vector<VertexShader> vss;
		std::vector<PixelShader> pss;
	};
}
