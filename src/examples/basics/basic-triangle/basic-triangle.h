#pragma once

#include <vector>

#include "examples/i-example.h"
#include "core/context.h"
#include "systems/i-system.h"

namespace basicExample {
	/**
	 * @brief The most simple exemple. Render a triangle on screen
	 */
	class BasicTriangle : public IExample {
	public:
		BasicTriangle(Context& context);
		virtual ~BasicTriangle();

		virtual void update() override;
		virtual void imGuiUpdate() override;

	private:
		Context& m_ctx;
		std::vector<std::shared_ptr<ISystem>> m_systems;
	};
}
