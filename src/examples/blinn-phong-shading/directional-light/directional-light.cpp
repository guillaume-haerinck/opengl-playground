#include "directional-light.h"

#include <imgui.h>

#include "systems/render-system.h"
#include "systems/camera-system.h"
#include "factories/entities/model-factory.h"
#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "components/physics/transform.h"

namespace blinnPhongShadingExample {
	DirectionalLight::DirectionalLight(Context& context) : m_ctx(context) {
		// Init
		m_systems = {
			std::make_shared<RenderSystem>(context),
			std::make_shared<CameraSystem>(context)
		};

        ModelFactory modelFactory(context);

		// Pipeline
		scomp::ShaderPipeline shaders = {};
		shaders.vs = m_ctx.rcommand->createVertexShader("res/shaders/blinn-phong-shading/directional-light/directional-light.vert");
		shaders.fs = m_ctx.rcommand->createFragmentShader("res/shaders/blinn-phong-shading/directional-light/directional-light.frag");
		scomp::ConstantBufferIndex cbIndices[] = {
			scomp::PER_MESH,
			scomp::PER_FRAME
		};
		comp::Pipeline pipeline = m_ctx.rcommand->createPipeline(shaders, cbIndices, std::size(cbIndices));

		// Transform
		comp::Transform transform = {};

		// Assign pipeline to model entites
		auto entities = modelFactory.createEntitiesFromGltf("res/models/damaged-helmet/DamagedHelmet.gltf");
		for (auto entity : entities) {
			m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
			m_ctx.registry.assign<comp::Transform>(entity, transform);
		}
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	void DirectionalLight::update() {
		for (auto& system : m_systems) {
			system->update();
		}
	}

	void DirectionalLight::imGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Directional light");
		ImGui::End();
	}
}
