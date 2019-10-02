#include "rotating-cube.h"

#include <imgui.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <glm/glm.hpp>

#include "systems/render-system.h"
#include "graphics/gl-exception.h"
#include "graphics/vertex-input-description.h"
#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "components/physics/transform.h"

namespace basicExample {
	struct perCustomChanges { // must be multiple of 16 bytes
		glm::vec3 color;
		char padding[2];
	};

	RotatingCube::RotatingCube(Context& context) : m_ctx(context) {
        // Init
		m_systems = {
			std::make_shared<RenderSystem>(context)
		};

		// Vertex buffer
		VertexInputDescription inputDescription = {
			{ ShaderDataType::Float2, "Position" }
		};

		float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.0f,  0.5f
		};
		comp::AttributeBuffer positionBuffer = m_ctx.rcommand->createAttributeBuffer(&positions, std::size(positions), sizeof(float));
		comp::VertexBuffer vertexBuffer = m_ctx.rcommand->createVertexBuffer(inputDescription, &positionBuffer, 1);

		// Index buffer
		unsigned int indices[] = { 0, 1, 2 };
		comp::IndexBuffer indexBuffer = m_ctx.rcommand->createIndexBuffer(indices, std::size(indices));

		// Shaders
		scomp::VertexShader vs = m_ctx.rcommand->createVertexShader("res/shaders/basics/rotating-cube/rotating-cube.vert");
		scomp::FragmentShader fs = m_ctx.rcommand->createFragmentShader("res/shaders/basics/rotating-cube/rotating-cube.frag");

		// Custom constant buffer
		scomp::ConstantBuffers& cbs = m_ctx.registry.get<scomp::ConstantBuffers>(m_ctx.singletonComponents.at(scomp::SING_ENTITY_GRAPHIC));
		perCustomChanges cbData = {};
		cbData.color = glm::vec3(0.0f, 0.0f, 1.0f);
		scomp::ConstantBuffer cb = m_ctx.rcommand->createConstantBuffer(sizeof(perCustomChanges), "perCustomChanges", &cbData);
		cbs.constantBuffers.at(scomp::ConstantBufferIndex::PER_CUSTOM_PROP_CHANGE_0) = cb;

		// Pipeline
		scomp::ConstantBufferIndex cbIndices[] = {
			scomp::PER_CUSTOM_PROP_CHANGE_0
		};
		comp::Pipeline pipeline = m_ctx.rcommand->createPipeline(vs, fs, cbIndices, std::size(cbIndices));
		
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
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void RotatingCube::imGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Rotating cube");
		ImGui::End();
	}
}
