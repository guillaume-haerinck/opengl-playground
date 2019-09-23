#pragma once

#include <SDL2/SDL.h>
#include "core/context.h"

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

private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    Context m_ctx;
    static bool m_instanciated;
    bool m_running;
};
