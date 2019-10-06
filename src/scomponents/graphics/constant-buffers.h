#pragma once

#include <array>
#include <string>

namespace scomp {
	/**
	 * @brief Constant buffers used by shader. This structure is used to update constant buffers.
	 */
	struct ConstantBuffer {
		unsigned int bufferId;
		unsigned int byteWidth = 0;
		std::string name;
		// TODO keep track of the layout
	};

	/**
	 * @note The layouts are accessible on the graphics/ConstantBuffer.h file
	 */
	enum ConstantBufferIndex {
		PER_MESH = 0,
		PER_MESH_BATCH,
		PER_FRAME,
		PER_LIGHT_CHANGE,
		PER_COOK_MAT_CHANGE,
		PER_PHONG_MAT_CHANGE,
		PER_CUSTOM_PROP_CHANGE_0, // Updated by example
		PER_CUSTOM_PROP_CHANGE_1, // Updated by example
		PER_CUSTOM_PROP_CHANGE_2, // Updated by example
		_CONST_BUFFER_MAX
	};
}
