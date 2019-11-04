#include "instanced-rendering.h"

#include <imgui.h>

#include "systems/render-system.h"
#include "systems/camera-system.h"
#include "factories/components/mesh-primitive-factory.h"
#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "components/physics/transform.h"

namespace advancedExample {
	InstancedRendering::InstancedRendering(Context& context) : m_ctx(context) {
		// Init
		m_systems = {
			std::make_shared<RenderSystem>(context),
			std::make_shared<CameraSystem>(context)
		};
		MeshPrimitiveFactory primFactory(context);

		// Pipeline
		scomp::ShaderPipeline shaders = {};
		shaders.vs = m_ctx.rcommand->createVertexShader("res/shaders/advanced/instanced-rendering/instanced-rendering.vert");
		shaders.fs = m_ctx.rcommand->createFragmentShader("res/shaders/advanced/instanced-rendering/instanced-rendering.frag");
		scomp::ConstantBufferIndex cbIndices[] = {
			scomp::PER_MESH,
			scomp::PER_FRAME
		};
		comp::Pipeline pipeline = m_ctx.rcommand->createPipeline(shaders, cbIndices, std::size(cbIndices));

		// Mesh
		comp::Mesh mesh = primFactory.createBox(2);

		// Transform
		comp::Transform transform = {};

		// Assign data to entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
		m_ctx.registry.assign<comp::Transform>(entity, transform);

		auto entity2 = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Mesh>(entity2, mesh);
		m_ctx.registry.assign<comp::Pipeline>(entity2, pipeline);
		transform.position = glm::vec3(1);
		m_ctx.registry.assign<comp::Transform>(entity2, transform);
	}

	InstancedRendering::~InstancedRendering()
	{
	}

	void InstancedRendering::update() {
		for (auto& system : m_systems) {
			system->update();
		}
	}

	void InstancedRendering::imGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Instanced rendering");
		ImGui::End();
	}
}
