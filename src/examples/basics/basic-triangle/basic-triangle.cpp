#include "basic-triangle.h"

#include <imgui.h>
#include <glad/glad.h>

#include "graphics/gl-exception.h"

namespace basicExample {
	BasicTriangle::BasicTriangle(Context& context) : m_ctx(context) {
		// Vertex array
		GLuint va;
		GLCall(glGenVertexArrays(1, &va));
		GLCall(glBindVertexArray(va));

		// Attribute buffer
		const GLfloat positions[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f,  1.0f, 0.0f
		};
		GLuint vb;
		GLCall(glGenBuffers(1, &vb));
		GLCall(glBindBuffer(GL_PACK_ALIGNMENT, vb));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

		// 1rst attribute buffer : positions
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
		GLCall(glVertexAttribPointer(
			0,                  // attribute 0
			3,                  // number of vertices
			GL_FLOAT,           // type
			GL_FALSE,           // IsNormalized
			0,                  // stride
			(void*) 0           // array buffer offset
		));

		const char* vsSource = R"(
			#version 300 es

			layout(location = 0) in vec3 position;

			void main() {
				gl_Position = vec4(position, 1.0);
			}
		)";

		const char* fragSource = R"(
			#version 300 es
			layout(location = 0) out lowp vec4 color;

			void main() {
				color = vec4(1, 0, 0, 1);
			}
		)";
	}

	BasicTriangle::~BasicTriangle() {}

	void BasicTriangle::Update() {
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

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
