#include "rotating-cube.h"

#include <imgui.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>

#include "graphics/gl-exception.h"
#include "graphics/vertex-input-description.h"
#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "components/physics/transform.h"

namespace basicExample {
	RotatingCube::RotatingCube(Context& context) : m_ctx(context) {
		// Vertex buffer
		float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.0f,  0.5f
		};
		comp::AttributeBuffer positionBuffer = m_ctx.rcommand->createAttributeBuffer(&positions, std::size(positions), sizeof(float));
		comp::VertexBuffer vertexBuffer = m_ctx.rcommand->createVertexBuffer(&positionBuffer, 1);

		// Index buffer
		unsigned int indices[] = { 0, 1, 2 };
		comp::IndexBuffer indexBuffer = m_ctx.rcommand->createIndexBuffer(indices, std::size(indices));

		// Pipeline
		VertexInputDescription inputDescription = {
			{ ShaderDataType::Float2, "Position" }
		};
		inputDescription.vertexArrayId = vertexBuffer.vertexArrayId;
		scomp::VertexShader vs = m_ctx.rcommand->createVertexShader("res/shaders/basics/rotating-cube/rotating-cube.vert", inputDescription);
		scomp::FragmentShader fs = m_ctx.rcommand->createFragmentShader("res/shaders/basics/rotating-cube/rotating-cube.frag");
		comp::Pipeline pipeline = m_ctx.rcommand->createPipeline(vs, fs);

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

	RotatingCube::~RotatingCube() {}

	void RotatingCube::update() {
		m_ctx.registry.view<comp::Mesh, comp::Pipeline, comp::Transform>()
			.each([&](comp::Mesh& mesh, comp::Pipeline& pipeline, comp::Transform& transform) {
			// Bind
			m_ctx.rcommand->bindPipeline(pipeline);
			m_ctx.rcommand->bindVertexBuffer(mesh.vb);
			m_ctx.rcommand->bindIndexBuffer(mesh.ib);

			// Draw call
			m_ctx.rcommand->drawIndexed(mesh.ib.count);
		});
	}

	void RotatingCube::imGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("First example window");
		ImGui::End();
	}
}
