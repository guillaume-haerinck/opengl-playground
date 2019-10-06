#include "camera-system.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scomponents/io/inputs.h"
#include "scomponents/graphics/camera.h"

CameraSystem::CameraSystem(Context& context) : m_ctx(context)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::update() {
    // Get singleton components
	// scomp::Inputs& inputs = m_ctx.registry.get<scomp::Inputs>(ioEntity);
	//scomp::Camera& camera = m_ctx.registry.get<scomp::Camera>(graphicEntity);

    /*
	// ArcBall rotation
	if (inputs.actionState.at(scomp::InputAction::CAM_ORBIT)) {
		camera.theta -= inputs.delta.x * 0.01;
		camera.phi += inputs.delta.y * 0.01;

		// Keep phi within -2PI to +2PI for easy 'up' comparison
		if (camera.phi > glm::two_pi<float>())   {
			camera.phi -= glm::two_pi<float>();
		} else if (camera.phi < -glm::two_pi<float>()) {
			camera.phi += glm::two_pi<float>();
		}

		// If phi is between 0 to PI or -PI to -2PI, make 'up' be positive Y, other wise make it negative Y
		if ((camera.phi > 0 && camera.phi < glm::pi<float>()) || (camera.phi < -glm::pi<float>() && camera.phi > -glm::two_pi<float>())) {
			camera.up = 1.0f;
		} else {
			camera.up = -1.0f;
		}

		camera.hasToBeUpdated = true;
	}

	// Move along vertical plane
	if (inputs.actionState.at(scomp::InputAction::CAM_PAN)) {
		camera.hasToBeUpdated = true;
	}

	// Change arcball radius
	if (inputs.actionState.at(scomp::InputAction::CAM_DOLLY)) {
		if (inputs.wheelDelta > 0 && camera.radius >= 2) {
			camera.radius -= 1;
		} else if (inputs.wheelDelta < 0) {
			camera.radius += 1;
		}

		camera.hasToBeUpdated = true;
	}

	// Reset zoom and position
	if (inputs.actionState.at(scomp::InputAction::CAM_RESET)) {
		camera.phi = 0;
		camera.theta = 0;
		camera.radius = 10;
		camera.up = 1;
		camera.target = glm::vec3(0, 0, 0);

		camera.hasToBeUpdated = true;
	}

	// Update camera position
	if (camera.hasToBeUpdated == true) {
		camera.position.x = camera.radius * sinf(camera.phi) * sinf(camera.theta);
		camera.position.y = camera.radius * cosf(camera.phi);
		camera.position.z = camera.radius * sinf(camera.phi) * cosf(camera.theta);

        glm::vec3 eye = camera.position;
        glm::vec3 target = camera.target;
        glm::vec3 up = glm::vec3(0, camera.up, 0);

        camera.view = glm::lookAtLH(eye, target, up);
		camera.hasToBeUpdated = false;
	}
    */
}
