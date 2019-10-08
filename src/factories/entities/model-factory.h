#pragma once

#include "components/graphics/mesh.h"
#include "core/context.h"

#include <vector>

class ModelFactory {
public:
    ModelFactory(Context& context);
    ~ModelFactory();

    /**
	 * @brief
	 */
	std::vector<entt::entity> createEntitiesFromGltf(const char* gltfFilePath);

private:
    Context& m_ctx;
};
