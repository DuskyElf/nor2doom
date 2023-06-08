#include <raylib.h>

#include "./main.h"
#include "./logic_sim/logic_sim.h"

int main(void) {
    GlobalState global_state = {
        .screen_width = 1920,
        .screen_height = 1080,
        .font_size = 64,
    };

    SimCompList sim_comp_list = SimCompList_alloc();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(global_state.screen_width, global_state.screen_height, "Nor2Doom");

    SetTargetFPS(60);
    unsigned int eval_count = 0;
    while (!WindowShouldClose()) {
        global_state.screen_width = GetScreenWidth();
        global_state.screen_height = GetScreenHeight();
        SimCompList_eval_all(&sim_comp_list, eval_count);

        BeginDrawing();
            Logic_Sim_draw(&global_state);
        EndDrawing();
        eval_count++;
    }

    SimCompList_free(&sim_comp_list);

    return 0;
}
