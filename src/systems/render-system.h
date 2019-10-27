#pragma once

#include <glm/glm.hpp>

#include "i-system.h"
#include "core/context.h"
#include "components/physics/transform.h"
#include "components/graphics/mesh.h"
#include "components/graphics/pipeline.h"

class RenderSystem : public ISystem {
public:
	RenderSystem(Context& context);
	virtual ~RenderSystem();

	void update() override;

private:
	void addTempInstanceData(const comp::Transform& transform);
	void updateAndDrawInstance(const comp::Mesh& mesh, const comp::Pipeline& pipeline);

private:
	Context& m_ctx;
	unsigned int m_instanceCount;
	std::vector<glm::mat4> m_tempModelMats;
};
