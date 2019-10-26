#include "instanced-rendering.h"

#include <imgui.h>

namespace advancedExample {
	InstancedRendering::InstancedRendering(Context& context) : m_ctx(context) {
	}

	InstancedRendering::~InstancedRendering()
	{
	}

	void InstancedRendering::update() {
		for (auto& system : m_systems) {
			system->update();
		}
	}

	void InstancedRendering::imGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("Instanced rendering");
		ImGui::End();
	}
}
