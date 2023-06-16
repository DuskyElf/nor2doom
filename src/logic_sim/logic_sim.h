#ifndef _LOGIC_SIM_H
#define _LOGIC_SIM_H 

#include <raylib.h>
#include <stddef.h>

#include "../main.h"

#define MAX_CHILDREN 8
#define SIMCOMPLIST_INITIAL_SIZE 256

typedef enum {
    SC_BUFF = 0,
    SC_NOT,
    SC_OR,
    SC_AND,
} SimCompKind;

typedef enum {
    SC_IN_A = 0,
    SC_IN_B,
} SimCompPinKind;

typedef struct {
    struct SimComp* comp;
    SimCompPinKind pin;
} SimOutConn;

typedef struct {
    bool value;
    struct SimComp* comp;
    size_t self_i;
} SimInConn;

typedef struct {
    size_t count;
    SimOutConn data[MAX_CHILDREN];
} SimOutConnArray;

typedef struct SimComp {
    Vector2 position;
    SimCompKind kind;
    SimInConn a;
    SimInConn b;
    SimOutConnArray children;
    unsigned int eval_count;
} SimComp;

typedef struct {
    size_t count;
    size_t capacity;
    SimComp* data;
} SimCompList;

#define S_Conn(c, p) (SimOutConn) { .comp = (c), .pin = (p) }
#define S_Comp(k) (SimComp) { .kind = (k), .a = {0}, .b = {0}, .children = {0}, .eval_count = 0 }

const char* SimCompKind_text(SimCompKind self);

size_t SimOCA_add(SimOutConnArray* self, SimOutConn conn);
void SimOCA_remove(SimOutConnArray* self, size_t item_i);

void SimComp_inject(SimComp* self, SimOutConn conn);
void SimComp_eject(SimComp* self);
void SimComp_eval(SimComp* self, unsigned int eval_count);
Rectangle SimComp_get_rect(SimComp* self, int gs);

bool SimCompKind_is_binary(const SimCompKind kind);

SimCompList SimCompList_alloc();
void SimCompList_free(SimCompList* self);
void SimCompList_add(SimCompList* self, SimComp item);
void SimCompList_remove(SimCompList* self, size_t index);
void SimCompList_eval_all(SimCompList* self, unsigned int eval_count);

void Logic_Sim_update(SimCompList* comp_list, GlobalState global_state, int eval_count);

#endif // !_LOGIC_SIM_H
