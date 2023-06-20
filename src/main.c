#include <raylib.h>

#include "./main.h"
#include "./logic_sim/logic_sim.h"

int main(void) {
    GlobalState global_state = {
        .screen_width = 1920,
        .screen_height = 1080,
        .gui_scale = 10,
    };
    int gs = global_state.gui_scale;

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
    SimComp_inject(&sim_comp_list.data[7], S_Conn(&sim_comp_list.data[1], SC_IN_A));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(global_state.screen_width, global_state.screen_height, "Nor2Doom");

    SimComp_set_pos(&sim_comp_list.data[0], CLITERAL(Vector2) { 300, 300 }, gs);
    SimComp_set_pos(&sim_comp_list.data[1], CLITERAL(Vector2) { 300, 500 }, gs);
    SimComp_set_pos(&sim_comp_list.data[2], CLITERAL(Vector2) { 550, 300 }, gs);
    SimComp_set_pos(&sim_comp_list.data[3], CLITERAL(Vector2) { 550, 500 }, gs);
    SimComp_set_pos(&sim_comp_list.data[4], CLITERAL(Vector2) { 800, 200 }, gs);
    SimComp_set_pos(&sim_comp_list.data[5], CLITERAL(Vector2) { 800, 600 }, gs);
    SimComp_set_pos(&sim_comp_list.data[6], CLITERAL(Vector2) { 1000, 400 }, gs);
    SimComp_set_pos(&sim_comp_list.data[7], CLITERAL(Vector2) { 1200, 400 }, gs);
    //SimCompList_add(&sim_comp_list, S_Comp(SC_AND));

    sim_comp_list.data[0].a.value = 1;
    SetTargetFPS(60);
    unsigned int eval_count = 0;
    while (!WindowShouldClose()) {
        global_state.screen_width = GetScreenWidth();
        global_state.screen_height = GetScreenHeight();

        BeginDrawing();
            Logic_Sim_update(&sim_comp_list, global_state, eval_count);
        EndDrawing();

        eval_count++;
    }

    SimCompList_free(&sim_comp_list);

    return 0;
}
