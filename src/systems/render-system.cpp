#include "render-system.h"

#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "components/physics/transform.h"
#include "scomponents/graphics/camera.h"
#include "graphics/constant-buffer.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::update() {
    ///////////////////////////////////////////////////////////////////////////
	/////////////////////////////// NON-OPTIONAL //////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    // Update per frame constant buffer
	{
		cb::perFrame cbData;
        scomp::ConstantBuffer& perFrameCB = m_ctx.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME);

        // Set data
        cbData.cameraPos = m_ctx.camera.position;
        cbData.matViewProj =  m_ctx.camera.proj * m_ctx.camera.view;

        // Send data
		m_ctx.rcommand->updateConstantBuffer(perFrameCB, &cbData);
	}

    // Get singleton components used for rendering
	scomp::ConstantBuffer& perMeshCB = m_ctx.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH);

    m_ctx.registry.view<comp::Mesh, comp::Pipeline, comp::Transform>()
        .each([&](comp::Mesh& mesh, comp::Pipeline& pipeline, comp::Transform& transform) {

        // TODO check boolean to see if instanced. Entities needs to be sorted by mesh hash.
        // If same than before, add position inside a vector, and skip draw call.
        // If different, update the vertex attribute buffer for instanced entities and send the draw call

        // How to know which buffer id to use ? Add a vector in VertexBuffer of enum (PerVertexAny / PerInstancePosition / PerInstanceColor / ...)

        // Update perMesh constant buffer
        {
            cb::perMesh cbData = {};
            cbData.matModel =  glm::rotate(glm::mat4x4(1.0f), 2.4f, glm::vec3(0, 1, 0));
            cbData.materialIndex = 1;
            m_ctx.rcommand->updateConstantBuffer(perMeshCB, &cbData);
        }

        // Bind
        m_ctx.rcommand->bindPipeline(pipeline);
        m_ctx.rcommand->bindVertexBuffer(mesh.vb);
        m_ctx.rcommand->bindIndexBuffer(mesh.ib);

        // Bind textures
		switch (mesh.materialType) {
		case scomp::MaterialType::PHONG: {
            const auto& material = m_ctx.phongMaterials.materials.at(mesh.materialIndex);
			for (const auto& texture : material.textures) {
				m_ctx.rcommand->bindTexture(texture);
			}
			break;
		}

		default:
			break;
		}

        // Draw call
        m_ctx.rcommand->drawIndexed(mesh.ib.count, mesh.ib.type);
    });
}
