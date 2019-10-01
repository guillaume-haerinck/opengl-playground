#pragma once

#include <glm/glm.hpp>

namespace scomp {
	/**
	 * @brief Arcball camera with DOF movement support. Uses polar coordinates.
	 *
	 * @link https://www.talisman.org/~erlkonig/misc/shoemake92-arcball.pdf
	 * @link https://pdfs.semanticscholar.org/5a62/af2ae372f03ff7d23708dfa111ad51221729.pdf - Six degree of freedom Control with a Two-Dimentional input device
	 * @link https://github.com/microsoft/DXUT/blob/master/Optional/DXUTcamera.h
	 * @link https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
	 * @link https://github.com/RichieSams/thehalflingproject/blob/master/source/scene/camera.h
	 * @link https://openclassrooms.com/fr/courses/167717-creez-des-programmes-en-3d-avec-opengl/166678-controle-avance-de-la-camera-partie-1-2
	 */
	struct Camera {
		glm::mat4x4 view;
		glm::mat4x4 proj;
		glm::vec3 position = { 0, 0, 6 };
		glm::vec3 target = { 0, 0, 0 };

		float radius = 5;
		float up = 1;
		float phi = 0;
		float theta = 0;

		bool hasToBeUpdated = false;
	};
}
