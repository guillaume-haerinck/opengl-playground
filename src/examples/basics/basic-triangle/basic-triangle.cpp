#include "basic-triangle.h"

#include <imgui.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>

#include "systems/render-system.h"
#include "graphics/gl-exception.h"
#include "graphics/vertex-input-description.h"
#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "components/physics/transform.h"

namespace basicExample {
	BasicTriangle::BasicTriangle(Context& context) : m_ctx(context) {
		// Init
		m_systems = {
			std::make_shared<RenderSystem>(context)
		};

		// Vertex buffer
		VertexInputDescription inputDescription = {
			{ ShaderDataType::Float2, "Position" }
		};

		float positions[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 0.0f,  1.0f
		};
		comp::AttributeBuffer positionBuffer = m_ctx.rcommand->createAttributeBuffer(&positions, std::size(positions), sizeof(float));
		comp::VertexBuffer vertexBuffer = m_ctx.rcommand->createVertexBuffer(inputDescription, &positionBuffer, 1);

		// Index buffer
		unsigned int indices[] = { 0, 1, 2 };
		comp::IndexBuffer indexBuffer = m_ctx.rcommand->createIndexBuffer(indices, std::size(indices));

		// Pipeline
		scomp::ShaderPipeline shaders = {};
		shaders.vs = m_ctx.rcommand->createVertexShader("res/shaders/basics/basic-triangle/basic-triangle.vert");
		shaders.fs = m_ctx.rcommand->createFragmentShader("res/shaders/basics/basic-triangle/basic-triangle.frag");
		comp::Pipeline pipeline = m_ctx.rcommand->createPipeline(shaders);

		// Mesh
		comp::Mesh mesh = {};
		mesh.vb = vertexBuffer;
		mesh.ib = indexBuffer;

		// Transform
		comp::Transform transform = {};

		// Assign data to entity
		auto entity = m_ctx.registry.create();
		m_ctx.registry.assign<comp::Mesh>(entity, mesh);
		m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
		m_ctx.registry.assign<comp::Transform>(entity, transform);
	}

	BasicTriangle::~BasicTriangle() {}

	void BasicTriangle::update() {
		for (auto& system : m_systems) {
			system->update();
		}
	}

	void BasicTriangle::imGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("First example window");
		ImGui::End();
	}
}
