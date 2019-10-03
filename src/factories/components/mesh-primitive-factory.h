#pragma once

#include "core/context.h"
#include "components/graphics/mesh.h"

class MeshPrimitiveFactory {
public:
    MeshPrimitiveFactory(Context& context);
    ~MeshPrimitiveFactory();

    comp::Mesh createBox(float width = 1, float height = 1);

private:
    Context& m_ctx;
};
