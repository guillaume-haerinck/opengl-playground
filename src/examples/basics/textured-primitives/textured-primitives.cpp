#include "textured-primitives.h"

#include <imgui.h>

#include "systems/render-system.h"
#include "systems/camera-system.h"

#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "components/physics/transform.h"
#include "factories/components/mesh-primitive-factory.h"

namespace basicExample {
	TexturedPrimitives::TexturedPrimitives(Context& context) : m_ctx(context) {
        // Init
		m_systems = {
			std::make_shared<RenderSystem>(context),
			std::make_shared<CameraSystem>(context)
		};
		MeshPrimitiveFactory primFactory(context);

		// Pipeline
		scomp::ShaderPipeline shaders = {};
		shaders.vs = m_ctx.rcommand->createVertexShader("res/shaders/basics/textured-primitives/textured-primitives.vert");
		shaders.fs = m_ctx.rcommand->createFragmentShader("res/shaders/basics/textured-primitives/textured-primitives.frag");
		scomp::ConstantBufferIndex cbIndices[] = {
			scomp::PER_MESH,
			scomp::PER_FRAME
		};
		comp::Pipeline pipeline = m_ctx.rcommand->createPipeline(shaders, cbIndices, std::size(cbIndices));
		
		// Mesh
		comp::Mesh mesh = primFactory.createBox();

		// Material
		scomp::PhongMaterial material = {};
		scomp::Texture texture = m_ctx.rcommand->createTexture(scomp::PhongTexSlot::DIFFUSE, "res/textures/uv-grid.jpg");
		material.textures.push_back(texture);
		m_ctx.phongMaterials.materials.push_back(material);
		m_ctx.phongMaterials.hasToBeUpdated = true;
		mesh.materialType = scomp::MaterialType::PHONG;
		mesh.materialIndex = m_ctx.phongMaterials.materials.size() - 1;

		// Transform
		comp::Transform transform = {};

		// Assign data to entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
		m_ctx.registry.assign<comp::Transform>(entity, transform);
	}

	TexturedPrimitives::~TexturedPrimitives() {}

	void TexturedPrimitives::update() {
		for (auto& system : m_systems) {
			system->update();
		}
	}

	void TexturedPrimitives::imGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Textured primitives");
		ImGui::End();
	}
}
