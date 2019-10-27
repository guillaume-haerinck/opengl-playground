#pragma once

#include "core/context.h"
#include "components/graphics/mesh.h"

class MeshPrimitiveFactory {
public:
    MeshPrimitiveFactory(Context& context);
    ~MeshPrimitiveFactory();

    comp::Mesh createBox(unsigned int maxInstanceCount = 1, float width = 1, float height = 1);

    VertexInputDescription getInputDescription() const { return m_vib; }
	VertexInputDescription getInputDescriptionInstanced() const { return m_vibInstanced; }

private:
    Context& m_ctx;
    VertexInputDescription m_vib;
	VertexInputDescription m_vibInstanced;
};
