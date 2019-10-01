#pragma once

#include <array>
#include <vector>

namespace scomp {
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
		unsigned int vsIndex = 0;
		unsigned int fsIndex = 0;
		unsigned int gsIndex = 0;
		unsigned int programIndex = 0;
	};

	struct Pipelines {
		std::vector<Pipeline> pipelines;
	};
};
