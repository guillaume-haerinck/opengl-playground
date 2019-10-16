#include "model-factory.h"

#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <string.h>

#include "scomponents/graphics/materials.h"

ModelFactory::ModelFactory(Context& context) : m_ctx(context) {
	m_vib = {
		{ ShaderDataType::Float3, "Position" },
		{ ShaderDataType::Float3, "Normal" },
        { ShaderDataType::Float2, "TexCoord" },
    };
}

ModelFactory::~ModelFactory() {}

std::vector<entt::entity> ModelFactory::createEntitiesFromGltf(const std::string& gltfFilePath) {
    fx::gltf::Document doc;

	// Get directory
	std::string directory;
	const size_t last_slash_idx = gltfFilePath.rfind('/');
	if (std::string::npos != last_slash_idx) {
		directory = gltfFilePath.substr(0, last_slash_idx);
	}

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
			GltfBufferInfo positionBufferInfo = {};
			GltfBufferInfo normalBufferInfo = {};
			GltfBufferInfo texCoord0BufferInfo = {};
			GltfBufferInfo tangentBufferInfo = {};

			// Get attribute buffers
			// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#meshes
			for (auto const& attrib : primitive.attributes) {
				if (attrib.first == "POSITION") { // FLOAT3
					positionBufferInfo = getGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} else if (attrib.first == "NORMAL") { // FLOAT3
					normalBufferInfo = getGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} else if (attrib.first == "TEXCOORD_0") { // FLOAT2
					texCoord0BufferInfo = getGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} else if (attrib.first == "TANGENT") { // FLOAT4
					tangentBufferInfo = getGltfBufferInfo(doc, doc.accessors[attrib.second]);
				} 
			}

			// Create attribute buffers
			comp::AttributeBuffer positionBuffer = m_ctx.rcommand->createAttributeBuffer((void*) positionBufferInfo.data, positionBufferInfo.vertexCount, positionBufferInfo.dataStride);
			comp::AttributeBuffer normalBuffer = m_ctx.rcommand->createAttributeBuffer((void*) normalBufferInfo.data, normalBufferInfo.vertexCount, normalBufferInfo.dataStride);
			comp::AttributeBuffer texCoordBuffer = m_ctx.rcommand->createAttributeBuffer((void*) texCoord0BufferInfo.data, texCoord0BufferInfo.vertexCount, texCoord0BufferInfo.dataStride);

			// Create vertex buffer
			comp::AttributeBuffer attributeBuffers[] = {
				positionBuffer, normalBuffer, texCoordBuffer
			};
			comp::VertexBuffer vb = m_ctx.rcommand->createVertexBuffer(m_vib, attributeBuffers);

			// Get and create index buffer
			GltfBufferInfo indexBufferInfo = {};
			indexBufferInfo = getGltfBufferInfo(doc, doc.accessors[primitive.indices]);
			comp::IndexBuffer ib = m_ctx.rcommand->createIndexBuffer((void*) indexBufferInfo.data, indexBufferInfo.vertexCount, comp::IndexBuffer::dataType::UNSIGNED_SHORT);

			// Create the mesh
			comp::Mesh mesh = {};
			mesh.vb = vb;
			mesh.ib = ib;

			// Get material
			if (primitive.material >= 0) {
				fx::gltf::Material gltfMaterial = doc.materials[primitive.material];

				// Base color texture
				int32_t baseColorTexIndex = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
				std::string textureName = doc.images.at(baseColorTexIndex).uri;
				std::string texturePath = directory + "/" + textureName;
				
				// Material
				scomp::PhongMaterial material = {};
				scomp::Texture texture = m_ctx.rcommand->createTexture(scomp::PhongTexSlot::DIFFUSE, texturePath.c_str());
				material.textures.push_back(texture);
				m_ctx.phongMaterials.materials.push_back(material);
				m_ctx.phongMaterials.hasToBeUpdated = true;
				mesh.materialType = scomp::MaterialType::PHONG;
				mesh.materialIndex = m_ctx.phongMaterials.materials.size() - 1;

			} else {
				mesh.materialType = scomp::MaterialType::NO_MATERIAL;
			}

			// Create new entity
			auto entity = m_ctx.registry.create();
			entities.push_back(entity);
			m_ctx.registry.assign<comp::Mesh>(entity, mesh);
        }
    }

    return entities;
}

ModelFactory::GltfBufferInfo ModelFactory::getGltfBufferInfo(fx::gltf::Document const& doc, fx::gltf::Accessor const& accessor) {
	fx::gltf::BufferView const& bufferView = doc.bufferViews[accessor.bufferView];
	fx::gltf::Buffer const& buffer = doc.buffers[bufferView.buffer];

	uint32_t dataTypeSize = calculateDataTypeSize(accessor);
	return GltfBufferInfo{ &accessor, &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset], dataTypeSize, accessor.count * dataTypeSize, accessor.count };
}

uint32_t ModelFactory::calculateDataTypeSize(fx::gltf::Accessor const& accessor) {
	uint32_t elementSize = 0;
	switch (accessor.componentType) {
	case fx::gltf::Accessor::ComponentType::Byte:
	case fx::gltf::Accessor::ComponentType::UnsignedByte:
		elementSize = 1;
		break;
	case fx::gltf::Accessor::ComponentType::Short:
	case fx::gltf::Accessor::ComponentType::UnsignedShort:
		elementSize = 2;
		break;
	case fx::gltf::Accessor::ComponentType::Float:
	case fx::gltf::Accessor::ComponentType::UnsignedInt:
		elementSize = 4;
		break;
	}

	switch (accessor.type) {
	case fx::gltf::Accessor::Type::Mat2:
		return 4 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Mat3:
		return 9 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Mat4:
		return 16 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Scalar:
		return elementSize;
		break;
	case fx::gltf::Accessor::Type::Vec2:
		return 2 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Vec3:
		return 3 * elementSize;
		break;
	case fx::gltf::Accessor::Type::Vec4:
		return 4 * elementSize;
		break;
	}

	return 0;
}
