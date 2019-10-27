#pragma once

#include <assert.h>
#include <string>
#include <vector>

enum class ShaderDataType {
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

enum class BufferElementUsage {
	PerVertex = 0, PerInstance
};

static uint32_t shaderDataTypeSize(ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:    return 4;
	case ShaderDataType::Float2:   return 4 * 2;
	case ShaderDataType::Float3:   return 4 * 3;
	case ShaderDataType::Float4:   return 4 * 4;
	case ShaderDataType::Mat3:     return 4 * 3 * 3;
	case ShaderDataType::Mat4:     return 4 * 4 * 4;
	case ShaderDataType::Int:      return 4;
	case ShaderDataType::Int2:     return 4 * 2;
	case ShaderDataType::Int3:     return 4 * 3;
	case ShaderDataType::Int4:     return 4 * 4;
	case ShaderDataType::Bool:     return 1;
	}

	assert(false && "Unknown ShaderDataType!");
	return 0;
}

struct BufferElement {
	std::string name;
	ShaderDataType type;
	uint32_t size;
	bool normalized;
	BufferElementUsage usage;

	BufferElement() {}

	BufferElement(ShaderDataType type, const std::string& name, BufferElementUsage usage = BufferElementUsage::PerVertex, bool normalized = false)
		: name(name), type(type), size(shaderDataTypeSize(type)), usage(usage), normalized(normalized)
	{}

	uint32_t getComponentCount() const
	{
		switch (type)
		{
		case ShaderDataType::Float:   return 1;
		case ShaderDataType::Float2:  return 2;
		case ShaderDataType::Float3:  return 3;
		case ShaderDataType::Float4:  return 4;
		case ShaderDataType::Mat3:    return 3 * 3;
		case ShaderDataType::Mat4:    return 4 * 4;
		case ShaderDataType::Int:     return 1;
		case ShaderDataType::Int2:    return 2;
		case ShaderDataType::Int3:    return 3;
		case ShaderDataType::Int4:    return 4;
		case ShaderDataType::Bool:    return 1;
		}

		assert(false && "Unknown ShaderDataType!");
		return 0;
	}
};

class VertexInputDescription {
public:
	VertexInputDescription() {}

	VertexInputDescription(const std::initializer_list<BufferElement>& elements) : m_element(elements) {}

	inline const std::vector<BufferElement>& getElements() const { return m_element; }

	std::vector<BufferElement>::iterator begin() { return m_element.begin(); }
	std::vector<BufferElement>::iterator end() { return m_element.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_element.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_element.end(); }

private:
	std::vector<BufferElement> m_element;
};
