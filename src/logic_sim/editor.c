#include <raylib.h>
#include <assert.h>

#include "./editor.h"

struct EditorState {
    int selected_comp_i;
    Vector2 knife_start;
};

static struct EditorState editor_state = {0};

void Editor_draw_conn(SimOutConn conn, int gs) {
    // Rectangle child_rect = SimComp_get_rect(conn.comp, gs);
    //
    // bool conn_value;
    // Vector2 in_pin_pos;
    // switch (conn.pin) {
    //     case SC_IN_A:
    //         conn_value = conn.comp->a.value;
    //         in_pin_pos = a_pin_pos(child_rect, SimCompKind_is_binary(conn.comp->kind), gs);
    //         break;
    //     case SC_IN_B:
    //         conn_value = conn.comp->b.value;
    //         in_pin_pos = b_pin_pos(child_rect, gs);
    //         break;
    //     default:
    //         assert(0 && "Unreachable");
    // }
    // Color color = conn_value ? MY_COLOR_5 : MY_COLOR_6;
    //
    // DrawCircleV(out_pin_pos(parent_rect, gs), E_Joint_Radius(gs), color);
    // DrawLineEx(out_pin_pos(parent_rect, gs), in_pin_pos, E_Box_Thickness(gs), color);

    const SimInConn* drawalbe_conn;
    switch (conn.pin) {
        case SC_IN_A:
            drawalbe_conn = &conn.comp->a;
            break;
        case SC_IN_B:
            drawalbe_conn = &conn.comp->b;
            break;
        default:
            assert(0 && "Unreachable");
    }

    assert(drawalbe_conn->wire_count > 0);
    bool conn_value = drawalbe_conn->value;
    Vector2 in_pin_pos = drawalbe_conn->wires[0].start;
    Vector2 out_pin_pos = drawalbe_conn->wires[drawalbe_conn->wire_count - 1].end;
    Color color = conn_value ? MY_COLOR_5 : MY_COLOR_6;
    DrawCircleV(out_pin_pos, E_Joint_Radius(gs), color);
    DrawLineEx(out_pin_pos, in_pin_pos, E_Box_Thickness(gs), color);
}

void Editor_draw_comp(SimComp* comp, GlobalState global_state) {
    const int gs = global_state.gui_scale;
    Rectangle box_rect = SimComp_get_rect(comp, gs);
    const char* lable_text = SimCompKind_text(comp->kind);

    int lable_x = comp->position.x + E_Padding(gs);
    int lable_y = comp->position.y + E_Padding(gs);

    DrawRectangleLinesEx(box_rect, E_Box_Thickness(gs), MY_COLOR_3);
    DrawText(lable_text, lable_x, lable_y, E_Lable_Size(gs), MY_COLOR_4);

    Color a_color = comp->a.value ? MY_COLOR_5 : MY_COLOR_6;
    Color b_color = comp->b.value ? MY_COLOR_5 : MY_COLOR_6;

    DrawCircleV(comp->a.wires[0].start, E_Joint_Radius(gs), a_color);
    if (SimCompKind_is_binary(comp->kind)) {
        DrawCircleV(comp->b.wires[0].start, E_Joint_Radius(gs), b_color);
    }

    for (size_t i = 0; i < comp->children.count; ++i) {
        Editor_draw_conn(comp->children.data[i], gs);
    }
}

void Editor_interactions(SimCompList* comp_list, int gs) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (editor_state.selected_comp_i) {
            editor_state.selected_comp_i = 0;
        }
        else {
            for (size_t i = 1; i <= comp_list->count; ++i) {
                SimComp* comp = SL_at(comp_list, i);
                Rectangle comp_rect = SimComp_get_rect(comp, gs);
                if (CheckCollisionPointRec(GetMousePosition(), comp_rect)) {
                    editor_state.selected_comp_i = i;
                    break;
                }
            }
        }

    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        editor_state.knife_start = GetMousePosition();
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        Vector2 knife_end = GetMousePosition();
        for (size_t i = 0; i < comp_list->count; ++i) {
        }
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        DrawLineEx(editor_state.knife_start, GetMousePosition(), E_Knife_Thickness(gs), RED);
    }

    if (editor_state.selected_comp_i) {
        SimComp* selected_comp = SL_at(comp_list, editor_state.selected_comp_i);
        Vector2 m_delta = GetMouseDelta();
        SimComp_set_pos(
            selected_comp,
            CLITERAL(Vector2) {
                selected_comp->position.x + m_delta.x,
                selected_comp->position.y + m_delta.y,
            }, gs
        );
    }
}

void Editor_draw_comps(SimCompList comps, GlobalState global_state) {
    for (size_t i = 0; i < comps.count; ++i) {
        Editor_draw_comp(&comps.data[i], global_state);
    }
}
