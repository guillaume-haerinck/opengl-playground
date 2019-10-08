#include "rotating-cube.h"

#include <imgui.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <glm/glm.hpp>

#include "systems/render-system.h"
#include "systems/camera-system.h"

#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "components/physics/transform.h"
#include "factories/components/mesh-primitive-factory.h"

namespace basicExample {
	RotatingCube::RotatingCube(Context& context) : m_ctx(context) {
        // Init
		m_systems = {
			std::make_shared<RenderSystem>(context),
			std::make_shared<CameraSystem>(context)
		};
		MeshPrimitiveFactory primFactory(context);

		// Pipeline
		scomp::ShaderPipeline shaders = {};
		shaders.vs = m_ctx.rcommand->createVertexShader("res/shaders/basics/rotating-cube/rotating-cube.vert");
		shaders.fs = m_ctx.rcommand->createFragmentShader("res/shaders/basics/rotating-cube/rotating-cube.frag");
		scomp::ConstantBufferIndex cbIndices[] = {
			scomp::PER_MESH,
			scomp::PER_FRAME
		};
		comp::Pipeline pipeline = m_ctx.rcommand->createPipeline(shaders, cbIndices, std::size(cbIndices));
		
		// Mesh
		comp::Mesh mesh = primFactory.createBox();

		// Transform
		comp::Transform transform = {};

		// Assign data to entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
		m_ctx.registry.assign<comp::Transform>(entity, transform);
	}

	RotatingCube::~RotatingCube() {}

	void RotatingCube::update() {
		for (auto& system : m_systems) {
			system->update();
		}
	}

	void RotatingCube::imGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Rotating cube");
		ImGui::End();
	}
}
