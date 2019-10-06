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
        scomp::ConstantBuffer& perFrameCB = m_ctx.registry.ctx<scomp::ConstantBuffers>()
			.constantBuffers.at(scomp::ConstantBufferIndex::PER_FRAME);

        // Set data
        glm::mat4x4 proj = glm::perspectiveFovLH(glm::quarter_pi<float>(), 500.0f, 500.0f, 0.1f, 100.0f);
		glm::mat4x4 view = glm::translate(glm::mat4x4(1.0f), glm::vec3(0.0f, 0.0f, 6.0f));
        cbData.cameraPos = glm::vec3(1.0f, 0.0f, 0.0f);
        cbData.matViewProj = proj * view;

        // Send data
		m_ctx.rcommand->updateConstantBuffer(perFrameCB, &cbData);
	}

    // Get singleton components used for rendering
	scomp::ConstantBuffer& perMeshCB = m_ctx.registry.ctx<scomp::ConstantBuffers>()
		.constantBuffers.at(scomp::ConstantBufferIndex::PER_MESH);

    m_ctx.registry.view<comp::Mesh, comp::Pipeline, comp::Transform>()
        .each([&](comp::Mesh& mesh, comp::Pipeline& pipeline, comp::Transform& transform) {

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

        // TODO check instance count to do instanced drawing
        // TODO update perMesh if instance == 1, else if > 1 update perMeshBatch (where to store this data ? Use singleton component and an index to it ?)

        // Draw call
        m_ctx.rcommand->drawIndexed(mesh.ib.count);
    });
}
