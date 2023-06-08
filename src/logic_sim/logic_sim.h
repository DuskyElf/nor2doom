#ifndef _LOGIC_SIM_H
#define _LOGIC_SIM_H 

#include <stddef.h>

#include "../main.h"

#define MAX_CHILDREN 8

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
    SimOutConn as[MAX_CHILDREN];
} SimOutConnArray;

typedef struct SimComp {
    SimCompKind kind;
    SimInConn a;
    SimInConn b;
    SimOutConnArray children;
    unsigned int eval_count;
    bool is_freeing;
} SimComp;

#define SCONN(c, p) (SimOutConn) { .comp = (c), .pin = (p) }

size_t SimOCA_add(SimOutConnArray* self, SimOutConn conn);
void SimOCA_remove(SimOutConnArray* self, size_t item_i);

SimComp* SimComp_alloc();
void SimComp_free(SimComp* self, bool recursive_call);
SimComp* SimComp_append(SimComp* self, SimCompKind oc_kind, SimCompPinKind pin);
void SimComp_add_as_child(SimComp* self, SimOutConn conn);
void SimComp_eval(SimComp* self, unsigned int eval_count);

void Logic_Sim_draw(GlobalState* global_state);

#endif // !_LOGIC_SIM_H
