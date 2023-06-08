#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "./logic_sim.h"

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
                break;
            case SC_IN_B:
                child_conn.comp->b.comp = NULL;
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

    if (self->a.comp)
        SimComp_eval(self->a.comp, eval_count);
    if (self->b.comp)
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
    memmove(self->data + index, self->data + index + 1, self->count - index - 1);
    --self->count;
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
