#include "basic-triangle.h"

#include <imgui.h>

namespace basicExample {
	BasicTriangle::BasicTriangle(Context& context) : m_ctx(context) {
		
	}

	BasicTriangle::~BasicTriangle() {
        
	}

	void BasicTriangle::Update() {
		for (auto& system : m_systems) {
			system->Update();
		}
	}

	void BasicTriangle::ImGuiUpdate() {
		ImGui::Begin("Exemple properties");
		ImGui::Text("First example window");
		ImGui::End();
	}
}
