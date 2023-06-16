#include <raylib.h>
#include <assert.h>

#include "./editor.h"

static Vector2 a_pin_pos(Rectangle box_rect, bool is_binary, int gs) {
    int y_divisor = is_binary ? 3 : 2;
    return CLITERAL(Vector2) {
        .x = box_rect.x + E_Box_Thickness(gs) / 2.,
        .y = box_rect.y + box_rect.height / y_divisor,
    };
}

static Vector2 b_pin_pos(Rectangle box_rect, int gs) {
    return  CLITERAL(Vector2) {
        .x = box_rect.x + E_Box_Thickness(gs) / 2.,
        .y = box_rect.y + box_rect.height * 2 / 3,
    };
}

static Vector2 out_pin_pos(Rectangle box_rect, int gs) {
    return CLITERAL(Vector2) {
        .x = box_rect.x + box_rect.width + E_Box_Thickness(gs) / 2.,
        .y = box_rect.y + box_rect.height / 2,
    };
}

void Editor_draw_conn(SimOutConn conn, Rectangle parent_rect, int gs) {
    Rectangle child_rect = SimComp_get_rect(conn.comp, gs);

    bool conn_value;
    Vector2 in_pin_pos;
    switch (conn.pin) {
        case SC_IN_A:
            conn_value = conn.comp->a.value;
            in_pin_pos = a_pin_pos(child_rect, SimCompKind_is_binary(conn.comp->kind), gs);
            break;
        case SC_IN_B:
            conn_value = conn.comp->b.value;
            in_pin_pos = b_pin_pos(child_rect, gs);
            break;
        default:
            assert(0 && "Unreachable");
    }
    Color color = conn_value ? MY_COLOR_5 : MY_COLOR_6;

    DrawCircleV(out_pin_pos(parent_rect, gs), E_Joint_Radius(gs), color);
    DrawLineEx(out_pin_pos(parent_rect, gs), in_pin_pos, E_Box_Thickness(gs), color);
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

    if (SimCompKind_is_binary(comp->kind)) {
        DrawCircleV(a_pin_pos(box_rect, true, gs), E_Joint_Radius(gs), a_color);
        DrawCircleV(b_pin_pos(box_rect, gs), E_Joint_Radius(gs), b_color);
    } else {
        DrawCircleV(a_pin_pos(box_rect, false, gs), E_Joint_Radius(gs), a_color);
    }

    for (size_t i = 0; i < comp->children.count; ++i) {
        Editor_draw_conn(comp->children.data[i], box_rect, gs);
    }
}

void Editor_interactions(SimCompList* comp_list, int gs) {
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return;
    for (size_t i = 0; i < comp_list->count; ++i) {
        SimComp* comp = &comp_list->data[i];
        Rectangle comp_rect = SimComp_get_rect(comp, gs);
        if (CheckCollisionPointRec(GetMousePosition(), comp_rect)) {
            Vector2 m_delta = GetMouseDelta();
            comp->position.x += m_delta.x;
            comp->position.y += m_delta.y;
            return;
        }
    }
}

void Editor_draw_comps(SimCompList comps, GlobalState global_state) {
    for (size_t i = 0; i < comps.count; ++i) {
        Editor_draw_comp(&comps.data[i], global_state);
    }
}
