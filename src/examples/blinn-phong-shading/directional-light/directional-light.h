#pragma once

#include <vector>

#include "examples/i-example.h"
#include "core/context.h"
#include "systems/i-system.h"

namespace blinnPhongShadingExample {
	class DirectionalLight : public IExample {
	public:
		DirectionalLight(Context& context);
		virtual ~DirectionalLight();

		virtual void update() override;
		virtual void imGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::shared_ptr<ISystem>> m_systems;
	};
}
