#ifndef _EDITOR_H
#define _EDITOR_H

#include "../main.h"
#include "./logic_sim.h"

#define E_Padding(gs)       ((gs) * 2)
#define E_Lable_Size(gs)    ((gs) * 6)
#define E_Joint_Radius(gs)  ((gs) * 1)
#define E_Box_Thickness(gs) ((gs) * 0.5)

void Editor_draw_conn(SimOutConn conn, Rectangle parent_rect, int gs);
void Editor_draw_comp(SimComp* comp, GlobalState global_state);
void Editor_interactions(SimCompList* comp_list, int gs);
void Editor_draw_comps(SimCompList comps, GlobalState global_state);

#endif // !_EDITOR_H
