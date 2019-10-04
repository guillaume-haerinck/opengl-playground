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
    auto graphEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_GRAPHIC);

    ///////////////////////////////////////////////////////////////////////////
	/////////////////////////////// NON-OPTIONAL //////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    // Update per frame constant buffer
	{
		cb::perFrame cbData;
        scomp::ConstantBuffer& perFrameCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
			.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME);

        // Set data
        cbData.cameraPos = glm::vec3(1.0f, 0.0f, 0.0f);
        cbData.matViewProj = glm::mat4x4(1.0f);

        // Send data
		m_ctx.rcommand->updateConstantBuffer(perFrameCB, &cbData);
	}

    // Get singleton components used for rendering
	scomp::ConstantBuffer& perMeshCB = m_ctx.registry.get<scomp::ConstantBuffers>(graphEntity)
		.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH);

    m_ctx.registry.view<comp::Mesh, comp::Pipeline, comp::Transform>()
        .each([&](comp::Mesh& mesh, comp::Pipeline& pipeline, comp::Transform& transform) {

        // Update perMesh constant buffer
        {
            cb::perMesh cbData = {};
            cbData.matModel = glm::mat4x4(1.0f);
            cbData.materialIndex = 1;
            m_ctx.rcommand->updateConstantBuffer(perMeshCB, &cbData);
        }

        // Bind
        m_ctx.rcommand->bindPipeline(pipeline);
        m_ctx.rcommand->bindVertexBuffer(mesh.vb);
        m_ctx.rcommand->bindIndexBuffer(mesh.ib);

        // TODO check instance count to do instanced drawing
        // TODO update perMesh if instance == 1, else if > 1 update perMeshBatch (where to store this data ? Use singleton component and an index to it ?)

        // Draw call
        m_ctx.rcommand->drawIndexed(mesh.ib.count);
    });
}
