#include <raylib.h>

#include "./main.h"
#include "./logic_sim/logic_sim.h"

int main(void) {
    GlobalState global_state = {
        .screen_width = 1920,
        .screen_height = 1080,
        .gui_scale = 10,
    };

    SimCompList sim_comp_list = SimCompList_alloc();
    SimCompList_add(&sim_comp_list, S_Comp(SC_BUFF));
    SimCompList_add(&sim_comp_list, S_Comp(SC_BUFF));
    SimCompList_add(&sim_comp_list, S_Comp(SC_NOT));
    SimCompList_add(&sim_comp_list, S_Comp(SC_NOT));
    SimCompList_add(&sim_comp_list, S_Comp(SC_AND));
    SimCompList_add(&sim_comp_list, S_Comp(SC_AND));
    SimCompList_add(&sim_comp_list, S_Comp(SC_OR));
    SimCompList_add(&sim_comp_list, S_Comp(SC_BUFF));

    SimComp_inject(&sim_comp_list.data[0], S_Conn(&sim_comp_list.data[4], SC_IN_A));
    SimComp_inject(&sim_comp_list.data[0], S_Conn(&sim_comp_list.data[3], SC_IN_A));
    SimComp_inject(&sim_comp_list.data[1], S_Conn(&sim_comp_list.data[2], SC_IN_A));
    SimComp_inject(&sim_comp_list.data[1], S_Conn(&sim_comp_list.data[5], SC_IN_B));
    SimComp_inject(&sim_comp_list.data[2], S_Conn(&sim_comp_list.data[4], SC_IN_B));
    SimComp_inject(&sim_comp_list.data[3], S_Conn(&sim_comp_list.data[5], SC_IN_A));
    SimComp_inject(&sim_comp_list.data[4], S_Conn(&sim_comp_list.data[6], SC_IN_A));
    SimComp_inject(&sim_comp_list.data[5], S_Conn(&sim_comp_list.data[6], SC_IN_B));
    SimComp_inject(&sim_comp_list.data[6], S_Conn(&sim_comp_list.data[7], SC_IN_A));
    //SimComp_inject(&sim_comp_list.data[7], S_Conn(&sim_comp_list.data[1], SC_IN_A));

    sim_comp_list.data[0].position = CLITERAL(Vector2) { 300, 300 };
    sim_comp_list.data[1].position = CLITERAL(Vector2) { 300, 500 };
    sim_comp_list.data[2].position = CLITERAL(Vector2) { 550, 300 };
    sim_comp_list.data[3].position = CLITERAL(Vector2) { 550, 500 };
    sim_comp_list.data[4].position = CLITERAL(Vector2) { 800, 200 };
    sim_comp_list.data[5].position = CLITERAL(Vector2) { 800, 600 };
    sim_comp_list.data[6].position = CLITERAL(Vector2) { 1000, 400 };
    sim_comp_list.data[7].position = CLITERAL(Vector2) { 1200, 400 };
    //SimCompList_add(&sim_comp_list, S_Comp(SC_AND));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(global_state.screen_width, global_state.screen_height, "Nor2Doom");

    bool a = 0;
    bool b = 0;

    sim_comp_list.data[0].a.value = 1;
    SetTargetFPS(60);
    unsigned int eval_count = 0;
    while (!WindowShouldClose()) {
        global_state.screen_width = GetScreenWidth();
        global_state.screen_height = GetScreenHeight();

        if (eval_count % 50 == 0) {
            if (a) {
                a = 0;
                b = !b;
            } else {
                a = 1;
            }
        }
        
        sim_comp_list.data[0].a.value = a;
        sim_comp_list.data[1].a.value = b;


        SimCompList_eval_all(&sim_comp_list, eval_count / 50);

        BeginDrawing();
            Logic_Sim_draw(sim_comp_list, global_state);
        EndDrawing();

        eval_count++;
    }

    SimCompList_free(&sim_comp_list);

    return 0;
}
