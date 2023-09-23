use raylib::prelude::*;

use crate::models::*;

pub mod models;
mod editor;
pub use editor::*;
pub use models::*;

impl SimCompKind {
    fn get_rect(&self, position: Vector2, gs: f32) -> Rectangle {
        Rectangle::new(
            position.x,
            position.y,
            measure_text(self.get_text(), lable_size(gs) as i32) as f32 + padding(gs) * 2.,
            lable_size(gs) + padding(gs) * 2.,
        )
    }

    fn a_pin_pos(&self, position: Vector2, gs: f32) -> Vector2 {
        let box_rect = self.get_rect(position, gs);
        let y_divisior: f32 = if self.is_binary() { 3. } else { 2. };
        Vector2::new(
            box_rect.x + box_thickness(gs) / 2.,
            box_rect.y + box_rect.height / y_divisior,
        )
    }

    fn b_pin_pos(&self, position: Vector2, gs: f32) -> Vector2 {
        let box_rect = self.get_rect(position, gs);
        Vector2::new(
            box_rect.x + box_thickness(gs) / 2.,
            box_rect.y + box_rect.height * 2. / 3.,
        )
    }

    fn out_pin_pos(&self, position: Vector2, gs: f32) -> Vector2 {
        let box_rect = self.get_rect(position, gs);
        Vector2::new(
            box_rect.x + box_rect.width + box_thickness(gs) / 2.,
            box_rect.y + box_rect.height / 2.,
        )
    }
}

impl Wire {
    fn draw_conn(&self, canvas: &mut RaylibDrawHandle, value: bool, gs: f32) {
        // Color a_color = comp->a.value ? MY_COLOR_5 : MY_COLOR_6;
        // Color b_color = comp->b.value ? MY_COLOR_5 : MY_COLOR_6;

        // DrawCircleV(comp->a.wires[0].start, E_Joint_Radius(gs), a_color);
        // if (SimCompKind_is_binary(comp->kind)) {
        //     DrawCircleV(comp->b.wires[0].start, E_Joint_Radius(gs), b_color);
        // }
        let color = if value { INDICATOR_ON_COLOR } else { INDICATOR_OFF_COLOR };

        canvas.draw_circle_v(self.start, joint_radius(gs), color);
    }
}

impl SimComp<'_> {
    pub fn set_comp_position(&mut self, new_pos: Vector2, gs: f32) {
        self.position = new_pos;

        let out_pin_pos = self.kind.out_pin_pos(self.position, gs);

        // TODO: Could this be pretty?
        if let Some(wire) = self.a.wires.first_mut() {
            wire.start = self.kind.a_pin_pos(self.position, gs);
        }
        if let Some(wire) = self.a.wires.last_mut() {
            wire.end = out_pin_pos;
        }
        if let Some(wire) = self.b.wires.first_mut() {
            wire.start = self.kind.b_pin_pos(self.position, gs);
        }
        if let Some(wire) = self.b.wires.last_mut() {
            wire.end = out_pin_pos;
        }

        for child_conn in self.out.iter_mut() {
            match child_conn.pin {
                Pin::A => {
                    if let Some(wire) = child_conn.comp.a.wires.last_mut() {
                        wire.end = out_pin_pos;
                    }
                }
                Pin::B => {
                    if let Some(wire) = child_conn.comp.b.wires.last_mut() {
                        wire.end = out_pin_pos;
                    }
                }
            }
        }
    }

    fn eval(&mut self, eval_count: usize) {
        // Recursion guard
        if self.eval_count > eval_count {
            return;
        }
        self.eval_count += 1;

        // Makes sure that the parents are evalued first
        if let Some((ref mut parent, ..)) = self.a.source {
            parent.eval(eval_count);
        }
        if let Some((ref mut parent, ..)) = self.b.source {
            parent.eval(eval_count);
        }

        // No point of evaluating if there's no children
        if self.out.is_empty() {
            return;
        }

        let a = self.a.value;
        let b = self.b.value;

        let out = match self.kind {
            SimCompKind::Buffer => a,
            SimCompKind::Not => !a,
            SimCompKind::Or => a | b,
            SimCompKind::And => a & b,
        };

        for child_conn in self.out.iter_mut() {
            match child_conn.pin {
                Pin::A => child_conn.comp.a.value = out,
                Pin::B => child_conn.comp.b.value = out,
            }
        }
    }

    fn draw(&self, canvas: &mut RaylibDrawHandle, gs: f32) {
        let box_rect = self.kind.get_rect(self.position, gs);
        let lable_text = self.kind.get_text();

        let lable_x = (self.position.x + padding(gs)) as i32;
        let lable_y = (self.position.y + padding(gs)) as i32;

        canvas.draw_rectangle_lines_ex(box_rect, box_thickness(gs) as i32, GATE_BOX_COLOR);
        canvas.draw_text(lable_text, lable_x, lable_y, lable_size(gs) as i32, GATE_LABLE_COLOR);

        for wire in self.a.wires.iter() {
            wire.draw_conn(canvas, self.a.value, gs);
        }
        for wire in self.b.wires.iter() {
            wire.draw_conn(canvas, self.b.value, gs);
        }
    }
}

impl LogicSim<'_> {
    pub fn update(&mut self, canvas: &mut RaylibDrawHandle, eval_count: usize, global_state: &GlobalState) {
        // self.interactions(canvas, global_state.gui_scale);
        self.eval_all(eval_count);
        self.draw_all(canvas, global_state);
    }

    pub fn add_new(&mut self, kind: SimCompKind, position: Vector2) {
        let new_comp = SimComp::new(kind, position);
        self.comps.push(new_comp);
    }

    #[track_caller] // This function could panic for Out of Bounds
    pub fn remove(&mut self, index: usize) {
        let mut comp = self.comps.remove(index);

        if let Some((SimComp { out, .. }, my_index)) = comp.a.source {
            out.remove(my_index);
        }

        if let Some((SimComp { out, .. }, my_index)) = comp.b.source {
            out.remove(my_index);
        }

        for child_conn in comp.out.iter_mut() {
            match child_conn.pin {
                Pin::A => {
                    child_conn.comp.a.source = None;
                }
                Pin::B => {
                    child_conn.comp.b.source = None;
                }
            }
        }
    }

    pub fn connect(&mut self, to: &mut SimComp, in_pin: Pin, from: &mut SimComp) {
        todo!()
       // let my_index = self.comps[from_id].out.len();
       // self.comps[from_id].out.push(SimOutConn::new(to_id, in_pin));

       // let wires = vec![];
       // match in_pin {
       //     Pin::A => {
       //         self.comps[to_id].a = Some(SimInConn::new(
       //             from_id, my_index, wires));
       //     }
       //     Pin::B => {
       //         self.comps[to_id].b = Some(SimInConn::new(
       //             from_id, my_index, wires));
       //     }
       // }
    }

    // #[allow(dead_code, unused_variables)]
    // fn interactions(&self, canvas: &mut RaylibDrawHandle, gs: f32) {
    //     if canvas.is_mouse_button_pressed(
    //         MouseButton::MOUSE_LEFT_BUTTON) {
    //         todo!()
    //     }

    //     todo!()
    // }

    fn eval_all(&mut self, eval_count: usize) {
        for comp in self.comps.iter_mut() {
            comp.eval(eval_count);
        }
    }

    fn draw_all(&self, canvas: &mut RaylibDrawHandle, global_state: &GlobalState) {
        canvas.clear_background(BACKGROUND_COLOR);

        for comp in self.comps.iter() {
            comp.draw(canvas, global_state.gui_scale);
        }

        canvas.draw_text(
            "Logic Simulator",
            (4_f32 * global_state.gui_scale) as i32,
            (4_f32 * global_state.gui_scale) as i32,
            (8_f32 * global_state.gui_scale) as i32,
            TITLE_COLOR
        );
    }
}
