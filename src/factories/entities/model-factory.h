#pragma once

#include "components/graphics/mesh.h"
#include "core/context.h"

#include <vector>
#include <fx/gltf.h>

class ModelFactory {
public:
    ModelFactory(Context& context);
    ~ModelFactory();

    /**
	 * @brief
	 */
	std::vector<entt::entity> createEntitiesFromGltf(const char* gltfFilePath);

private:
    struct GltfBufferInfo {
        fx::gltf::Accessor const* accessor;
        uint8_t const* data;
        uint32_t dataStride;
        uint32_t totalSize;
        uint32_t vertexCount;
    };

    struct GltfImageInfo {
        std::string fileName{};
        uint32_t binarySize{};
        uint8_t* binaryData{};
    };

	GltfBufferInfo getGltfBufferInfo(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor);
	uint32_t calculateDataTypeSize(fx::gltf::Accessor const& accessor);

private:
    Context& m_ctx;
    VertexInputDescription m_vib;
};
