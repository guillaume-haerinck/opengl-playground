#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include "core/context.h"
#include "examples/i-example.h"

/**
 * @brief Base root of the app
 */
class App {
public:
    App();
    ~App();

    void update();

public:
    SDL_Window* getWindow() const;
    bool isRunning() const;
    void exit();

private:
    void initSDL();
    void initImgui() const;

    void handleSDLEvents();
    void renderMenu();

    template<typename T>
	void resetAppTo() {
        m_isContextInit = false;
		m_activeExemple.reset();
        m_ctx.registry.reset();
		m_activeExemple = std::make_unique<T>(m_ctx);
        m_isContextInit = true;
	}

private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    Context m_ctx;
    std::unique_ptr<IExample> m_activeExemple;
    static bool m_instanciated;
    static bool m_isContextInit;
    bool m_running;
};
