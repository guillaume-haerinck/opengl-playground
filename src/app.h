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

    void initGraphicsSingletonComponents();
    void initIOSingletonComponents();
    void initConstantBuffers();

    void handleSDLEvents();
    void renderMenu();

    template<typename T>
	void resetAppTo() {
		m_activeExemple.reset();
        m_ctx.rcommand.reset();
        m_ctx.registry.reset();
        initGraphicsSingletonComponents();
        initIOSingletonComponents();
        m_ctx.rcommand = std::make_unique<RenderCommand>(m_ctx.registry);
        initConstantBuffers();
		m_activeExemple = std::make_unique<T>(m_ctx);
	}

private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    Context m_ctx;
    std::unique_ptr<IExample> m_activeExemple;
    static bool m_instanciated;
    bool m_running;
};
