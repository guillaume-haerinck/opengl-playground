#pragma once

#include "i-system.h"
#include "core/context.h"

class CameraSystem : public ISystem {
public:
    CameraSystem(Context& context);
    virtual ~CameraSystem();

	void update() override;

private:
	Context& m_ctx;
};
