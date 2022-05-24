#include "../includes/wanted.h"
#include <emscripten.h>

typedef int (*loops)(void *);

SDLX_Display    *display;
master          *game_master;
loops           gameLoops[4] = {
    start_loop,
    game_loop,
    end_loop,
    transition_screen,
};

void main_loop()
{
    SDLX_Render_Reset(display);
    SDLX_RenderQueue_FlushAll();
    SDLX_InputLoop();
    SDLX_Input_Update();
    SDLX_TimedLoop(gameLoops[game_master->state], game_master);
    SDLX_RenderAll(display);
    SDL_RenderPresent(display->renderer);
    SDLX_CapFPS();
}
int main()
{

    SDLX_Start(
        WINDOW_NAME,
        WINDOW_X,
        WINDOW_Y,
        WINDOW_H,
        WINDOW_W,
        0
    );
    display = SDLX_Display_Get();
    game_master = game_init();

    emscripten_set_main_loop(main_loop, 0, 1);
    cleanup_Game(game_master);
}