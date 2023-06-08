#include <raylib.h>

#include "./main.h"
#include "./logic_sim/logic_sim.h"

int main(void) {
    GlobalState global_state = {
        .screen_width = 1920,
        .screen_height = 1080,
        .font_size = 64,
    };

    SimComp* root_comp  = SimComp_alloc();

    SimComp* not_gate = SimComp_append(root_comp, SC_NOT, SC_IN_A);
    SimComp_add_as_child(not_gate, SCONN(root_comp, SC_IN_A));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(global_state.screen_width, global_state.screen_height, "Nor2Doom");

    SetTargetFPS(60);
    unsigned int eval_count = 0;
    while (!WindowShouldClose()) {
        global_state.screen_width = GetScreenWidth();
        global_state.screen_height = GetScreenHeight();
        SimComp_eval(root_comp, eval_count);

        BeginDrawing();
            Logic_Sim_draw(&global_state);
        EndDrawing();
        eval_count++;
    }

    SimComp_free(root_comp, 0);

    return 0;
}
