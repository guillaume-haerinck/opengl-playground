#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		std::cerr << "[SDL2] Unable to initialize SDL: " << SDL_GetError() << std::endl;
	}
	SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_GL_SetSwapInterval(1);

	SDL_Window* window = SDL_CreateWindow(
		"OpenGL Playground",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		500, 500,
		SDL_WINDOW_OPENGL
    );
	if (window == nullptr) {
        std::cerr << "[SDL2] Window is null: " << SDL_GetError() << std::endl;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        std::cerr << "[SDL2] OpenGL context is null: " <<  SDL_GetError() << std::endl;
    }

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cerr << "[Glad] Glad not init" << std::endl;
	}

    // Game loop
    bool isRunning = true;
    while (isRunning) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
        
            case SDL_KEYDOWN:
                break;

            case SDL_KEYUP:
                break;

            case SDL_MOUSEBUTTONDOWN:
                break;
            }
        }

		SDL_GL_SwapWindow(window);
    }

    return 0;
}
