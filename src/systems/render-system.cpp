#include "render-system.h"

#include "scomponents/graphics/camera.h"
#include "graphics/constant-buffer.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context), m_instanceCount(0) {
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

	// Loop data
	auto view = m_ctx.registry.view<comp::Mesh, comp::Pipeline, comp::Transform>();
	unsigned int i = 0;
	unsigned int iMax = view.size();
	unsigned int lastVAbound = 0;
	bool lastEntityWasInstanced = false;

    view.each([&](comp::Mesh& mesh, comp::Pipeline& pipeline, comp::Transform& transform) {
		const bool isFirstInstance = m_instanceCount == 0 && mesh.isInstanced;
		if (isFirstInstance) {
			addTempInstanceData(transform);
		} else if (lastEntityWasInstanced) {
			if (lastVAbound == mesh.vb.vertexArrayId) { // The last entity used the same mesh	
				addTempInstanceData(transform);
			} else { // The last entity used a different mesh. Last of its kind. Time to draw it.
				updateAndDrawInstance(mesh, pipeline);
			}
		}

		// Bind
		m_ctx.rcommand->bindPipeline(pipeline);
		m_ctx.rcommand->bindVertexBuffer(mesh.vb);
		m_ctx.rcommand->bindIndexBuffer(mesh.ib);
		lastVAbound = mesh.vb.vertexArrayId;

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

		if (mesh.isInstanced) {
			lastEntityWasInstanced = true;
			if (i == iMax - 1) { // Last entity of the frame, need to draw it now
				updateAndDrawInstance(mesh, pipeline);
			}
		} else {
			lastEntityWasInstanced = false;

			// Update perMesh constant buffer
			{
				cb::perMesh cbData = {};
				cbData.matModel = glm::rotate(glm::mat4x4(1.0f), 2.4f, glm::vec3(0, 1, 0));
				cbData.materialIndex = 1;
				m_ctx.rcommand->updateConstantBuffer(perMeshCB, &cbData);
			}

			// Draw call
			m_ctx.rcommand->drawIndexed(mesh.ib.count, mesh.ib.type);
		}
		i++;
    });
}

void RenderSystem::addTempInstanceData(const comp::Transform& transform) {
	m_instanceCount++;

	// Prepare data for instanced draw call
	// TODO only update if it needs to be
	{
		m_tempModelMats.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, m_instanceCount * 4)));
	}
}

void RenderSystem::updateAndDrawInstance(const comp::Mesh& mesh, const comp::Pipeline& pipeline) {
	for (const auto& buffer : mesh.vb.buffers) {
		switch (buffer.type) {
		case comp::AttributeBufferType::PER_INSTANCE_MODEL_MAT:
			m_ctx.rcommand->updateAttributeBuffer(buffer, m_tempModelMats.data(), sizeof(glm::mat4) * m_tempModelMats.size());
			m_tempModelMats.clear();
			break;

		default:
			break;
		}
	}

	// Draw call
	m_ctx.rcommand->drawIndexedInstances(mesh.ib.count, mesh.ib.type, m_instanceCount);
	m_instanceCount = 0;
}
