#include "app.h"
#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif
#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

void mainLoop(void* arg);

struct MainLoopArg {
	App* app;
};

int main(int argc, char *argv[]) {
	#ifdef _WIN32 // Check memory leaks
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	App app;
	MainLoopArg arg = {};
	arg.app = &app;

	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(mainLoop, &arg, 0, 1);
	#else
		while (app.isRunning()) {
			mainLoop(&arg);
		}
	#endif

	return 0;
}

void mainLoop(void* arg) {
	MainLoopArg* args = (MainLoopArg*) arg;
	// TODO handle deltatime and sleep
	args->app->update();
}
