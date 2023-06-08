#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "./logic_sim.h"

size_t SimOCA_add(SimOutConnArray* self, SimOutConn conn) {
    assert(self->count < MAX_CHILDREN);
    self->as[self->count] = conn;
    ++self->count;
    return self->count - 1;
}

void SimOCA_remove(SimOutConnArray* self, size_t item_i) {
    assert(item_i < MAX_CHILDREN);
    memmove(self->as + item_i, self->as + item_i + 1, self->count - item_i - 1);
    --self->count;
}

SimComp* SimComp_alloc() {
    // zero initialized is a valid first component
    SimComp* alloc = calloc(1, sizeof(SimComp));
    if (alloc) return alloc;

    fprintf(stderr, "SimComp_alloc: Could not malloc: %s\n", strerror(errno));
    exit(69);
}

void SimComp_free(SimComp* self, bool recursive_call) {
    if (self->is_freeing)
        return;

    if (recursive_call && self->a.comp && self->b.comp)
        return;

    self->is_freeing = true;

    // deregistering from the parents
    if (!recursive_call) {
        if (self->a.comp) {
            SimOCA_remove(&self->a.comp->children, self->a.self_i);
        }
        if (self->b.comp) {
            SimOCA_remove(&self->b.comp->children, self->b.self_i);
        }
    }

    // recursively freeing its children
    for (size_t i = 0; i < self->children.count; ++i) {
        SimComp_free(self->children.as[i].comp, true);
    }

    free(self);
}

SimComp* SimComp_append(SimComp* self, SimCompKind oc_kind, SimCompPinKind pin) {
    SimComp* new_comp = SimComp_alloc();
    new_comp->kind = oc_kind;

    SimComp_add_as_child(self, SCONN(new_comp, pin));
    return new_comp;
}

void SimComp_add_as_child(SimComp* self, SimOutConn conn) {
    size_t index = SimOCA_add(&self->children, conn);
    switch (conn.pin) {
        case SC_IN_A:
            conn.comp->a.comp = self;
            conn.comp->a.self_i = index;
            break;
        case SC_IN_B:
            conn.comp->b.comp = self;
            conn.comp->b.self_i = index;
            break;
        default:
            assert(0 && "Unreachable");
    }
}

void SimComp_eval(SimComp* self, unsigned int eval_count) {
    if (self->eval_count > eval_count) return;
    self->eval_count++;

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
        switch (self->children.as[i].pin) {
            case SC_IN_A:
                self->children.as[i].comp->a.value = out;
                break;
            case SC_IN_B:
                self->children.as[i].comp->b.value = out;
                break;
            default:
                assert(0 && "Unreachable");
        }
        SimComp_eval(self->children.as[i].comp, eval_count);
    }
}

const Color BACKGROUND_COLOR = CLITERAL(Color){ 0x19, 0x18, 0x25, 0xFF };
const Color TITLE_COLOR = CLITERAL(Color){ 0xE3, 0x84, 0xFF, 0xFF };

void Logic_Sim_draw(GlobalState* global_state) {
    ClearBackground(BACKGROUND_COLOR);
    DrawText(
        "Logic Simulator",
        100, 100,
        global_state->font_size,
        TITLE_COLOR
    );
}
