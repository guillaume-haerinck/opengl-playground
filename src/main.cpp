#include <glad/glad.h>
#include "app.h"
#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif
#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif



void gameLoop(void* data);

int main(int argc, char *argv[]) {
	#ifdef _WIN32 // Check memory leaks
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	App* app = new App();

	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(gameLoop, (void *) app, 0, 0);
	#else
		while (app->isRunning()) {
			gameLoop((void *) app);
		}
	#endif

	delete app;
	return 0;
}

void gameLoop(void* data) {
	// TODO handle deltatime and sleep
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	App* app = static_cast<App*>(data);

	app->update();

	SDL_GL_SwapWindow(app->getWindow());
}
