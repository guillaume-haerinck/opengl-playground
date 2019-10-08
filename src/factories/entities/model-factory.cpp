#include "model-factory.h"

#include <fx/gltf.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>

#include "scomponents/graphics/materials.h"
#include "components/graphics/mesh.h"

ModelFactory::ModelFactory(Context& context) : m_ctx(context) {}

ModelFactory::~ModelFactory() {}

std::vector<entt::entity> ModelFactory::createEntitiesFromGltf(const char* gltfFilePath) {
    fx::gltf::Document doc;
    try {
        doc = fx::gltf::LoadFromText(gltfFilePath);
    } catch(const std::exception& e) {
        spdlog::error("[ModelFactory] {} ", e.what());
        debug_break();
        assert(false);
    }
    
	std::vector<entt::entity> entities;
    for (auto const& meshInfo : doc.meshes) {
		for (auto const& primitive : meshInfo.primitives) {
			// GltfBufferInfo positionBufferInfo = {};
			// GltfBufferInfo normalBufferInfo = {};
			// GltfBufferInfo texCoord0BufferInfo = {};
			// GltfBufferInfo tangentBufferInfo = {};

			// Get attribute buffers
			// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#meshes
			for (auto const& attrib : primitive.attributes) {
				if (attrib.first == "POSITION") { // FLOAT3
					// positionBufferInfo = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} else if (attrib.first == "NORMAL") { // FLOAT3
					// normalBufferInfo = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} else if (attrib.first == "TEXCOORD_0") { // FLOAT2
					// texCoord0BufferInfo = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} else if (attrib.first == "TANGENT") { // FLOAT4
					// tangentBufferInfo = GetGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} 
			}
        }
    }

    return entities;
}
