#include "app.h"
#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif
#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

#include <entt/entt.hpp>

void mainLoop(void* arg);

struct MainLoopArg {
	App* app;
};

struct test {
	float x;
};

void freefonc(entt::entity entity, entt::registry & registry) {
	auto component = registry.get<test>(entity);
	component.x = 5;
}

int main(int argc, char *argv[]) {
	#ifdef _WIN32 // Check memory leaks
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	entt::registry registry;
	auto entity = registry.create();
	registry.on_destroy<test>().connect<&freefonc>();
	test mytest = {};
	registry.assign<test>(entity, mytest);
	registry.remove<test>(entity);

	App* app = new App();
	MainLoopArg arg = {};
	arg.app = app;

	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(mainLoop, &app, 0, 1);
	#else
		while (app->isRunning()) {
			mainLoop(&app);
		}
	#endif

	delete app;
	return 0;
}

void mainLoop(void* arg) {
	MainLoopArg* args = (MainLoopArg*) arg;
	// TODO handle deltatime and sleep
	args->app->update();
}
