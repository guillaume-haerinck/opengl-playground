#include "render-system.h"

#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"
#include "components/physics/transform.h"

RenderSystem::RenderSystem(Context& context) : m_ctx(context) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::Update() {
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
