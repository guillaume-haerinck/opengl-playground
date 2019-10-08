#include "model-loading.h"

#include <imgui.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <glm/glm.hpp>

#include "systems/render-system.h"
#include "systems/camera-system.h"

#include "components/graphics/pipeline.h"
#include "factories/entities/model-factory.h"

namespace basicExample {
	ModelLoading::ModelLoading(Context& context) : m_ctx(context) {
        // Init
		m_systems = {
			std::make_shared<RenderSystem>(context),
			std::make_shared<CameraSystem>(context)
		};
		ModelFactory modelFactory(context);

		// Pipeline
		scomp::ShaderPipeline shaders = {};
		shaders.vs = m_ctx.rcommand->createVertexShader("res/shaders/basics/rotating-cube/rotating-cube.vert");
		shaders.fs = m_ctx.rcommand->createFragmentShader("res/shaders/basics/rotating-cube/rotating-cube.frag");
		scomp::ConstantBufferIndex cbIndices[] = {
			scomp::PER_MESH,
			scomp::PER_FRAME
		};
		comp::Pipeline pipeline = m_ctx.rcommand->createPipeline(shaders, cbIndices, std::size(cbIndices));

		// Assign pipeline to model entites
		auto entities = modelFactory.createEntitiesFromGltf("res/models/damaged-helmet/DamagedHelmet.gltf");
		for (auto entity : entities) {
			m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
		}
	}

	ModelLoading::~ModelLoading() {}

	void ModelLoading::update() {
		for (auto& system : m_systems) {
			system->update();
		}
	}

	void ModelLoading::imGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Model loading");
		ImGui::End();
	}
}
