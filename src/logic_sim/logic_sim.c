#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "./logic_sim.h"
#include "./editor.h"

const char* SimCompKind_text(SimCompKind self) {
    char* text;
    switch (self) {
        case SC_BUFF:
            text = "Buff";
            break;
        case SC_NOT:
            text = "Not";
            break;
        case SC_OR:
            text = "Or";
            break;
        case SC_AND:
            text = "And";
            break;
        default:
            assert(0 && "Unreachable");
    }
    
    return text;
}

size_t SimOCA_add(SimOutConnArray* self, SimOutConn conn) {
    assert(self->count < MAX_CHILDREN);
    self->data[self->count] = conn;
    ++self->count;
    return self->count - 1;
}

void SimOCA_remove(SimOutConnArray* self, size_t item_i) {
    assert(item_i < MAX_CHILDREN);
    memmove(self->data + item_i, self->data + item_i + 1, self->count - item_i - 1);
    --self->count;
}

void SimComp_inject(SimComp* self, SimOutConn conn) {
    size_t index = SimOCA_add(&self->children, conn);
    SimInConn* drawable_conn;
    switch (conn.pin) {
        case SC_IN_A:
            drawable_conn = &conn.comp->a;
            break;
        case SC_IN_B:
            drawable_conn = &conn.comp->b;
            break;
        default:
            assert(0 && "Unreachable");
    }
    drawable_conn->comp = self;
    drawable_conn->self_i = index;
    drawable_conn->wire_count = 1;
}

void SimComp_eject(SimComp *self) {
    if (self->a.comp)
        SimOCA_remove(&self->a.comp->children, self->a.self_i);
    if (self->b.comp)
        SimOCA_remove(&self->b.comp->children, self->b.self_i);

    for (size_t i = 0; i < self->children.count; ++i) {
        SimOutConn child_conn = self->children.data[i];
        switch (child_conn.pin) {
            case SC_IN_A:
                child_conn.comp->a.comp = NULL;
                child_conn.comp->a.wire_count = 0;
                break;
            case SC_IN_B:
                child_conn.comp->b.comp = NULL;
                child_conn.comp->a.wire_count = 0;
                break;
            default:
                assert(0 && "Unreachable");
        }
    }
}

void SimComp_eval(SimComp* self, unsigned int eval_count) {
    if (self->eval_count > eval_count)
        return;

    self->eval_count++;

    if (self->a.comp && self->a.comp->position.x > self->position.x)
        SimComp_eval(self->a.comp, eval_count);
    if (self->b.comp && self->b.comp->position.x > self->position.x)
        SimComp_eval(self->b.comp, eval_count);

    if (self->children.count == 0)
        return;

    bool out;
    switch (self->kind) {
        case SC_BUFF:
            out = self->a.value;
            break;
        case SC_NOT:
            out = !self->a.value;
            break;
        case SC_OR:
            out = self->a.value || self->b.value;
            break;
        case SC_AND:
            out = self->a.value && self->b.value;
            break;
        default:
            assert(0 && "Unreachable");
    }

    for (size_t i = 0; i < self->children.count; ++i) {
        SimOutConn child_conn = self->children.data[i];
        switch (child_conn.pin) {
            case SC_IN_A:
                child_conn.comp->a.value = out;
                break;
            case SC_IN_B:
                child_conn.comp->b.value = out;
                break;
            default:
                assert(0 && "Unreachable");
        }
    }
}

void SimComp_set_pos(SimComp* self, Vector2 pos, int gs) {
    self->position = pos;

    if (self->a.wire_count) {
        self->a.wires[0].start = SimComp_a_pin_pos(self, gs);
        self->a.wires[self->a.wire_count - 1].end = SimComp_out_pin_pos(self->a.comp, gs);
    }

    if (self->b.wire_count) {
        self->b.wires[0].start = SimComp_b_pin_pos(self, gs);
        self->b.wires[self->b.wire_count - 1].end = SimComp_out_pin_pos(self->b.comp, gs);
    }

    for (size_t i = 0; i < self->children.count; ++i) {
        SimOutConn* child = &self->children.data[i];
        switch (child->pin) {
            case SC_IN_A:
                child->comp->a.wires[child->comp->a.wire_count - 1].end = SimComp_out_pin_pos(self, gs);
                break;
            case SC_IN_B:
                child->comp->b.wires[child->comp->b.wire_count - 1].end = SimComp_out_pin_pos(self, gs);
                break;
            default:
                assert(0 && "Unreachable");
        }
    }
}

Rectangle SimComp_get_rect(SimComp* self, int gs) {
    return (CLITERAL(Rectangle) {
        self->position.x,
        self->position.y,
        MeasureText(SimCompKind_text(self->kind), E_Lable_Size(gs)) + E_Padding(gs) * 2,
        E_Lable_Size(gs) + E_Padding(gs) * 2,
    });
}

Vector2 SimComp_a_pin_pos(SimComp* self, int gs) {
    bool is_binary = SimCompKind_is_binary(self->kind);
    Rectangle box_rect = SimComp_get_rect(self, gs);
    int y_divisor = is_binary ? 3 : 2;
    return CLITERAL(Vector2) {
        .x = box_rect.x + E_Box_Thickness(gs) / 2.,
        .y = box_rect.y + box_rect.height / y_divisor,
    };
}

Vector2 SimComp_b_pin_pos(SimComp* self, int gs) {
    Rectangle box_rect = SimComp_get_rect(self, gs);
    return  CLITERAL(Vector2) {
        .x = box_rect.x + E_Box_Thickness(gs) / 2.,
        .y = box_rect.y + box_rect.height * 2 / 3,
    };
}

Vector2 SimComp_out_pin_pos(SimComp* self, int gs) {
    Rectangle box_rect = SimComp_get_rect(self, gs);
    return CLITERAL(Vector2) {
        .x = box_rect.x + box_rect.width + E_Box_Thickness(gs) / 2.,
        .y = box_rect.y + box_rect.height / 2,
    };
}

bool SimCompKind_is_binary(const SimCompKind kind) {
    switch (kind) {
        case SC_BUFF:
            return false;
        case SC_NOT:
            return false;
        case SC_OR:
            return true;
        case SC_AND:
            return true;
    }
    assert(0 && "Unreachable");
}

SimCompList SimCompList_alloc() {
    SimComp* alloc = malloc(SIMCOMPLIST_INITIAL_SIZE * sizeof(SimComp));
    if (!alloc) {
        fprintf(stderr, "SimCompList_alloc: Could not malloc: %s\n", strerror(errno));
        exit(69);
    }

    return (SimCompList) {
        .count = 0,
        .capacity = SIMCOMPLIST_INITIAL_SIZE,
        .data = alloc,
    };
}

void SimCompList_free(SimCompList* self) {
    if (!self->capacity) {
        fprintf(stderr, "SimCompList_free: Double free detected\n");
        exit(69);
    }
    
    free(self->data);
    self->capacity = 0;
}

void SimCompList_add(SimCompList* self, SimComp item) {
    if (self->capacity < self->count) {
        fprintf(stderr, "SimCompList_add: Usage of freed memory or corruption detected\n");
        exit(69);
    }

    if (self->count == self->capacity) {
        self->capacity *= 2;
        self->data = realloc(self->data, self->capacity);
        if (!self->data) {
            fprintf(stderr, "SimComp_add: Could not realloc: %s\n", strerror(errno));
            exit(69);
        }
    }

    self->data[self->count] = item;
    ++self->count;
}

void SimCompList_eval_all(SimCompList* self, unsigned int eval_count) {
    for (size_t i = 0; i < self->count; ++i) {
        SimComp_eval(&self->data[i], eval_count);
    }
}

void SimCompList_remove(SimCompList* self, size_t index) {
    assert(index < self->count);
    SimComp_eject(&self->data[index]);
    memmove(self->data + index, self->data + index + 1, self->count - index - 1);
    --self->count;
}

void Logic_Sim_update(SimCompList* comp_list, GlobalState global_state, int eval_count) {
    Editor_interactions(comp_list, global_state.gui_scale);
    SimCompList_eval_all(comp_list, eval_count / 50);
    ClearBackground(MY_COLOR_1);
    Editor_draw_comps(*comp_list, global_state);
    DrawText(
        "Logic Simulator",
        4 * global_state.gui_scale,
        4 * global_state.gui_scale,
        8 * global_state.gui_scale,
        MY_COLOR_2
    );
}
