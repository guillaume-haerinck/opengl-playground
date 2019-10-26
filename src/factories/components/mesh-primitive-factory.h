#pragma once

#include "core/context.h"
#include "components/graphics/mesh.h"

class MeshPrimitiveFactory {
public:
    MeshPrimitiveFactory(Context& context);
    ~MeshPrimitiveFactory();

    comp::Mesh createBox(unsigned int instanceCount = 1, float width = 1, float height = 1);

    VertexInputDescription getInputDescription() const { return m_vib; }
	// TODO getInputDescriptionInstanced

private:
    Context& m_ctx;
    VertexInputDescription m_vib;
};
