#pragma once

namespace comp {
	/**
	 * @brief Store position, rotation and scaling of an entity in the world
	 */
	struct Transform {
		glm::vec3 position;
		float scale;
	};
}
