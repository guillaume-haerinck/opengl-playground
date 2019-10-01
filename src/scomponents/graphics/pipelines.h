#pragma once

#include <array>
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

	enum PipelineShaderIndex {
		VS = 0,
		FS,
		GS,
		_MAX_SHADER
	};

	/**
	 * @brief Store IDs to the shaders used by an entity. The IDs relate to the singleton component Shaders owned by the graphic entity
	 */
	struct Pipeline {
		std::array<bool, PipelineShaderIndex::_MAX_SHADER> hasShader = { true, true, false };
		FragmentShader fs;
		VertexShader vs;
		unsigned int programIndex = 0;
	};

	struct Pipelines {
		std::vector<Pipeline> pipelines;
	};
};
