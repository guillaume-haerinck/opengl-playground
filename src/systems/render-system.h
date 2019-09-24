#pragma once

#include "i-system.h"
#include "core/context.h"

class RenderSystem : public ISystem {
public:
	RenderSystem(Context& context);
	virtual ~RenderSystem();

	void Update() override;

private:
	Context& m_ctx;
};
