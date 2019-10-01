#pragma once

#include <array>
#include <vector>

#include "scomponents/graphics/constant-buffers.h"

namespace scomp {
	/**
	 * @brief A vertex shader. The first stage in the graphic pipeline.
	 */
	struct VertexShader {
		unsigned int shaderId;
	};

	/**
	 * @brief A pixel shader. The last stage in the graphic pipeline.
	 */
	struct FragmentShader {
		unsigned int shaderId;
	};

	enum PipelineShaderIndex {
		VS = 0,
		FS,
		GS,
		_MAX_SHADER
	};

	/**
	 * @brief 
	 */
	struct Pipeline {
		std::array<bool, PipelineShaderIndex::_MAX_SHADER> hasShader = { true, true, false };
		FragmentShader fs;
		VertexShader vs;
		unsigned int programIndex = 0;
		std::vector<scomp::ConstantBufferIndex> constantBufferIndices;
	};

	struct Pipelines {
		std::vector<Pipeline> pipelines;
	};
};
