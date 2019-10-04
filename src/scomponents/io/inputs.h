#pragma once

#include <glm/glm.hpp>
#include <array>

namespace scomp {
	enum InputAction {
		CAM_ORBIT = 0,
		CAM_PAN,
		CAM_DOLLY,
		CAM_RESET,
		_ACTION_MAX
	};

	struct Inputs {
		std::array<bool, InputAction::_ACTION_MAX> actionState;
		glm::vec2 mousePos = { 0, 0 };
		glm::vec2 delta = { 0, 0 };
		unsigned int wheelDelta = 0;
	};
}
