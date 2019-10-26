#include "mesh-primitive-factory.h"

#include <glm/glm.hpp>

MeshPrimitiveFactory::MeshPrimitiveFactory(Context& context) : m_ctx(context)
{
	m_vib = {
		{ ShaderDataType::Float3, "Normal" },
        { ShaderDataType::Float2, "TexCoord" },
		{ ShaderDataType::Float3, "Position" }
    };

	m_vibInstanced = {
		{ ShaderDataType::Float3, "Position" },
		{ ShaderDataType::Mat4, "Test", BufferElementUsage::PerInstance }
	};
}

MeshPrimitiveFactory::~MeshPrimitiveFactory()
{
}

comp::Mesh MeshPrimitiveFactory::createBox(unsigned int instanceCount, float width, float height) {
    //    v6----- v5
	//   /|      /|
	//  v1------v0|
	//  | |     | |
	//  | |v7---|-|v4
	//  |/      |/
	//  v2------v3
	glm::vec3 positions[] = {
		// Front v0,v1,v2,v3
		glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1),
		// Right v0,v3,v4,v5
		glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(1, -1, -1), glm::vec3(1, 1, -1),
		// Top v0,v5,v6,v1	
		glm::vec3(1, 1, 1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1), 
		// Left v1,v6,v7,v2	
		glm::vec3(-1, 1, 1), glm::vec3(-1, 1, -1), glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1),  
		// Bottom v7,v4,v3,v2
		glm::vec3(-1, -1, -1), glm::vec3(1, -1, -1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, 1), 
		// Back v4,v7,v6,v5	
		glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1)
	};
	
	glm::vec3 normals[] = {
		glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0),
		glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0),
		glm::vec3(0,-1, 0), glm::vec3(0,-1, 0), glm::vec3(0,-1, 0), glm::vec3(0,-1, 0),
		glm::vec3(0, 0,-1), glm::vec3(0, 0,-1), glm::vec3(0, 0,-1), glm::vec3(0, 0,-1)
	};

	glm::vec2 texCoords[] = {
		glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1),
		glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0),
		glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1),
		glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1),
		glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0),
		glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0)
	};

	unsigned short indices[] = {
		0, 1, 2,   2, 3, 0,       // front
		4, 5, 6,   6, 7, 4,       // right
		8, 9, 10,  10,11, 8,      // top
		12,13,14,  14,15,12,      // left
		16,17,18,  18,19,16,      // bottom
		20,21,22,  22,23,20		  // back
	};

    // Create buffers
	comp::AttributeBuffer positionBuffer = m_ctx.rcommand->createAttributeBuffer(positions, std::size(positions), sizeof(glm::vec3));
	comp::AttributeBuffer normalBuffer = m_ctx.rcommand->createAttributeBuffer(normals, std::size(normals), sizeof(glm::vec3));
	comp::AttributeBuffer texCoordBuffer = m_ctx.rcommand->createAttributeBuffer(texCoords, std::size(texCoords), sizeof(glm::vec2));
	comp::IndexBuffer ib = m_ctx.rcommand->createIndexBuffer(indices, std::size(indices), comp::IndexBuffer::dataType::UNSIGNED_SHORT);
	
	// Store buffers
	comp::VertexBuffer vertexBuffer = {};
	if (instanceCount > 1) {
		glm::mat4 testData[]{ glm::mat4(1.0f), glm::mat4(1.0f) };
		comp::AttributeBuffer testBuffer = m_ctx.rcommand->createAttributeBuffer(testData, std::size(testData), sizeof(glm::mat4));
		comp::AttributeBuffer attributeBuffers[] = {
			positionBuffer, testBuffer
		};
		vertexBuffer = m_ctx.rcommand->createVertexBuffer(m_vibInstanced, attributeBuffers);

	} else {
		comp::AttributeBuffer attributeBuffers[] = {
			normalBuffer, texCoordBuffer, positionBuffer
		};
		vertexBuffer = m_ctx.rcommand->createVertexBuffer(m_vib, attributeBuffers);
	}

    // Send result
    comp::Mesh mesh = {};
    mesh.vb = vertexBuffer;
    mesh.ib = ib;
	if (instanceCount > 1)
		mesh.isInstanced = true;

    return mesh;
}
