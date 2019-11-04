#include "app.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#include "graphics/gl-exception.h"
#include "graphics/constant-buffer.h"

#include "examples/basics/basic-triangle/basic-triangle.h"
#include "examples/basics/rotating-cube/rotating-cube.h"
#include "examples/basics/textured-primitives/textured-primitives.h"
#include "examples/basics/model-loading/model-loading.h"

#include "examples/blinn-phong-shading/directional-light/directional-light.h"

#include "examples/advanced/instanced-rendering/instanced-rendering.h"

bool App::m_instanciated = false;

App::App() : m_running(true)
{
    assert(!m_instanciated);
	m_instanciated = true;

    spdlog::set_pattern("[%l] %^ %v %$");

	initSDL();
    initImgui();

	GLCall(glEnable(GL_DEPTH_TEST));

	resetAppTo<advancedExample::InstancedRendering>();
}

App::~App() {
	m_ctx.rcommand.reset();
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
    SDL_DestroyWindow(m_window);
	SDL_Quit();
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// PUBLIC METHODS ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////


void App::update() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Feed inputs
	handleSDLEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	// Render our data
	m_activeExemple->update();
	GLCall(glBindVertexArray(0));

	// Update imgui
	renderMenu();
	m_activeExemple->imGuiUpdate();

	// Render imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Reset input deltas
	m_ctx.inputs.delta = glm::vec2(0.0f);
	m_ctx.inputs.wheelDelta = 0;

	SDL_GL_SwapWindow(m_window);
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PRIVATE METHODS ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////


void App::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		spdlog::critical("[SDL2] Unable to initialize SDL: {}", SDL_GetError());
		debug_break();
	}
	SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_GL_SetSwapInterval(1);

	m_window = SDL_CreateWindow(
		"OpenGL Playground",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		500, 500,
		SDL_WINDOW_OPENGL
    );
	if (m_window == nullptr) {
        spdlog::critical("[SDL2] Window is null: {}", SDL_GetError());
        debug_break();
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (m_glContext == nullptr) {
        spdlog::critical("[SDL2] OpenGL context is null: {}",  SDL_GetError());
        debug_break();
    }

	if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress)) {
		spdlog::critical("[Glad] Glad not init");
		debug_break();
	}
}

void App::initImgui() const {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
	ImGui_ImplOpenGL3_Init("#version 300 es");
	ImGui::StyleColorsDark();
}

void App::initConstantBuffers() {
	m_ctx.rcommand->createConstantBuffer(scomp::ConstantBufferIndex::PER_MESH, sizeof(cb::perMesh));
	m_ctx.rcommand->createConstantBuffer(scomp::ConstantBufferIndex::PER_FRAME, sizeof(cb::perFrame));
}

void App::handleSDLEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch (e.type) {
		case SDL_QUIT:
			exit();
			break;

		case SDL_MOUSEWHEEL:
			m_ctx.inputs.wheelDelta = e.wheel.y;
			m_ctx.inputs.actionState.at(scomp::InputAction::CAM_DOLLY) = true;
			break;

		case SDL_MOUSEMOTION: {
			int newPosX = e.button.x;
			int newPosY = e.button.y;
			m_ctx.inputs.delta.x = m_ctx.inputs.mousePos.x - newPosX;
			m_ctx.inputs.delta.y = m_ctx.inputs.mousePos.y - newPosY;
			m_ctx.inputs.mousePos.x = newPosX;
			m_ctx.inputs.mousePos.y = newPosY;
			break;
		}

		case SDL_MOUSEBUTTONDOWN:
			m_ctx.inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = true;
			break;

		case SDL_MOUSEBUTTONUP:
			m_ctx.inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = false;
			break;

		default:
			break;
		}
	}

	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		m_ctx.inputs.actionState.fill(false);
	}
}


void App::renderMenu() {
	ImGui::Begin("Main debug window");

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::CollapsingHeader("Help")) {
		ImGui::Text("Camera controls :");
		ImGui::BulletText("Orbit - Left mouse button / Middle mouse button");
		ImGui::BulletText("Pan - Right mouse button");
		ImGui::BulletText("Zoom - Mousewheel");
		ImGui::BulletText("Reset - Left mouse double click");
	}

	ImGui::Spacing();

	ImGui::Text("Exemples:");
	if (ImGui::CollapsingHeader("Basic")) {
		if (ImGui::Button("Basic triangle")) { resetAppTo<basicExample::BasicTriangle>(); }
		if (ImGui::Button("Rotating cube")) { resetAppTo<basicExample::RotatingCube>(); }
		if (ImGui::Button("Textured primitives")) { resetAppTo<basicExample::TexturedPrimitives>(); }
		if (ImGui::Button("Model loading")) { resetAppTo<basicExample::ModelLoading>(); }
	}

	if (ImGui::CollapsingHeader("Blinn Phong shading")) {
		if (ImGui::Button("Directional light")) { resetAppTo<blinnPhongShadingExample::DirectionalLight>(); }
	}

	// if (ImGui::CollapsingHeader("Intermediate")) {}

	if (ImGui::CollapsingHeader("Advanced")) {
		if (ImGui::Button("Instanced rendering")) { resetAppTo<advancedExample::InstancedRendering>(); }
	}

	// if (ImGui::CollapsingHeader("Toon shading")) {}

	// if (ImGui::CollapsingHeader("PBR shading")) {}

	ImGui::End();
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////// GETTERS & SETTERS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

SDL_Window* App::getWindow() const { return m_window; }
bool App::isRunning() const { return m_running; }
void App::exit() { m_running = false; }
