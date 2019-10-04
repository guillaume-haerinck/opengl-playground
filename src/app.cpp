#include "app.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#include "graphics/gl-exception.h"
#include "graphics/constant-buffer.h"

#include "scomponents/graphics/pipelines.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/camera.h"
#include "scomponents/io/inputs.h"

#include "examples/basics/basic-triangle/basic-triangle.h"
#include "examples/basics/rotating-cube/rotating-cube.h"

bool App::m_instanciated = false;

App::App() : m_running(true)
{
    assert(!m_instanciated);
	m_instanciated = true;

    spdlog::set_pattern("[%l] %^ %v %$");

	initSDL();
    initImgui();

	resetAppTo<basicExample::RotatingCube>();
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

void App::initGraphicsSingletonComponents() {
	entt::entity entity = m_ctx.registry.create();
	m_ctx.singletonComponents.at(scomp::SING_ENTITY_GRAPHIC) = entity;

	scomp::Pipelines pipelines = {};
	m_ctx.registry.assign<scomp::Pipelines>(entity, pipelines);

	scomp::ConstantBuffers cbs = {};
	m_ctx.registry.assign<scomp::ConstantBuffers>(entity, cbs);

	//scomp::Camera camera;
	//m_ctx.registry.assign<scomp::Camera>(entity, camera);
}

void App::initIOSingletonComponents() {
	entt::entity entity = m_ctx.registry.create();
	m_ctx.singletonComponents.at(scomp::SING_ENTITY_IO) = entity;

	//scomp::Inputs inputs = {};
	//m_ctx.registry.assign<scomp::Inputs>(entity, inputs);
}

void App::initConstantBuffers() {
	m_ctx.rcommand->createConstantBuffer(scomp::ConstantBufferIndex::PER_MESH, sizeof(cb::perMesh));
	m_ctx.rcommand->createConstantBuffer(scomp::ConstantBufferIndex::PER_MESH_BATCH, sizeof(cb::perMeshBatch));
	m_ctx.rcommand->createConstantBuffer(scomp::ConstantBufferIndex::PER_FRAME, sizeof(cb::perFrame));
}

void App::handleSDLEvents() {
	// auto ioEntity = m_ctx.singletonComponents.at(scomp::SingletonEntities::SING_ENTITY_IO);
	// scomp::Inputs& inputs = m_ctx.registry.get<scomp::Inputs>(ioEntity);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			exit();
			break;

		case SDL_MOUSEWHEEL:
			// inputs.wheelDelta = e.button.y;
			// inputs.actionState.at(scomp::InputAction::CAM_DOLLY) = true;
			break;

		case SDL_MOUSEMOTION: {
			//int newPosX = e.button.x;
			//int newPosY = e.button.y;
			//inputs.delta.x = inputs.mousePos.x - newPosX;
			//inputs.delta.y = inputs.mousePos.y - newPosY;
			//inputs.mousePos.x = newPosX;
			//inputs.mousePos.y = newPosY;
			break;
		}

		case SDL_MOUSEBUTTONDOWN:
			//inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = true;
			break;

		case SDL_MOUSEBUTTONUP:
			//inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = false;
			break;

		default:
			break;
		}
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
	}

	// if (ImGui::CollapsingHeader("Intermediate")) {}

	// if (ImGui::CollapsingHeader("Advanced")) {}

	// if (ImGui::CollapsingHeader("Blinn Phong shading")) { }

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
