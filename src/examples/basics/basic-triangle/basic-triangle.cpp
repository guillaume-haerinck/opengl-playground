#include "basic-triangle.h"

#include <imgui.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>

#include "graphics/gl-exception.h"

namespace basicExample {
	void checkShaderError(unsigned int id, unsigned int type) {
		// Error handling
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char *)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			auto const typeString = [type]() {
				switch (type) {
					case GL_VERTEX_SHADER: return "fragment";
					case GL_FRAGMENT_SHADER: return "vertex";
					default: return "unknown type";
				}
			}();

			spdlog::error("[Shader] Failed to compile {} shader", typeString);
			spdlog::error("[Shader] {}", message);
			debug_break();
			GLCall(glDeleteShader(id));
		}
	}

	BasicTriangle::BasicTriangle(Context& context) : m_ctx(context) {
		// Vertex array
		GLuint va;
		GLCall(glGenVertexArrays(1, &va));
		GLCall(glBindVertexArray(va));

		// Attribute buffer
		float positions[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f,  1.0f, 0.0f
		};
		comp::AttributeBuffer positionBuffer = m_ctx.rcommand->createAttributeBuffer(&positions, ARRAYSIZE(positions), sizeof(float));

		// Vertex buffer layout
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(
			0,                  // attribute 0
			3,                  // number of vertices
			GL_FLOAT,           // type
			GL_FALSE,           // IsNormalized
			0,                  // stride
			(void*) 0           // array buffer offset
		));

		const char* vsSource = R"(#version 300 es
			layout(location = 0) in vec3 position;

			void main() {
				gl_Position = vec4(position, 1.0);
			}
		)";

		const char* fragSource = R"(#version 300 es
			layout(location = 0) out lowp vec4 color;

			void main() {
				color = vec4(1, 0, 0, 1);
			}
		)";

		// Create pipeline
		{
			unsigned int program = glCreateProgram();
			unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
			GLCall(glShaderSource(vs, 1, &vsSource, nullptr));
			GLCall(glCompileShader(vs));
			checkShaderError(vs, GL_VERTEX_SHADER);

			unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
			GLCall(glShaderSource(fs, 1, &fragSource, nullptr));
			GLCall(glCompileShader(fs));
			checkShaderError(fs, GL_FRAGMENT_SHADER);

			GLCall(glAttachShader(program, vs));
			GLCall(glAttachShader(program, fs));
			GLCall(glLinkProgram(program));
			GLCall(glValidateProgram(program));

			GLCall(glDeleteShader(vs));
			GLCall(glDeleteShader(fs));

			GLCall(glUseProgram(program));
		}
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
