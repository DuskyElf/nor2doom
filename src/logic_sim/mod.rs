use raylib::prelude::*;

pub mod models;
mod editor;
pub use editor::*;
pub use models::*;

impl SimComp {
    fn get_rect(&self, gs: f32) -> Rectangle {
        Rectangle::new(
            self.position.x,
            self.position.y,
            measure_text(self.kind.get_text(), lable_size(gs) as i32) as f32,
            lable_size(gs) + padding(gs) * 2.,
        )
    }

    fn a_pin_pos(&self, gs: f32) -> Vector2 {
        let box_rect = self.get_rect(gs);
        let y_divisior: f32 =
            if self.kind.is_binary() { 3. } else { 2. };
        Vector2::new(
            box_rect.x + box_thickness(gs) / 2.,
            box_rect.y + box_rect.height / y_divisior,
        )
    }

    fn b_pin_pos(&self, gs: f32) -> Vector2 {
        let box_rect = self.get_rect(gs);
        Vector2::new(
            box_rect.x + box_thickness(gs) / 2.,
            box_rect.y + box_rect.height * 2. / 3.,
        )
    }

    fn out_pin_pos(&self, gs: f32) -> Vector2 {
        let box_rect = self.get_rect(gs);
        Vector2::new(
            box_rect.x + box_rect.width + box_thickness(gs) / 2.,
            box_rect.y + box_rect.height / 2.,
        )
    }
}

impl SimComps {
    pub fn add_new(&mut self, kind: SimCompKind, position: Vector2) {
        let new_comp = SimComp::new(kind, position);
        self.comps.push(new_comp);
    }

    pub fn remove(&mut self, index: usize) {
        if let Some(SimInConn {
            parent_id, my_index, ..
        }) = self.comps[index].a {
                self.comps[parent_id].out.remove(my_index);
            }

        if let Some(SimInConn {
            parent_id, my_index, ..
        }) = self.comps[index].b {
                self.comps[parent_id].out.remove(my_index);
            }

        for i in 0..self.comps[index].out.len() {
            let child_conn = self.comps[index].out[i];
            match child_conn.pin {
                Pin::A => {
                    self.comps[child_conn.comp_id].a = None;
                }
                Pin::B => {
                    self.comps[child_conn.comp_id].b = None;
                }
            }
        }

        self.comps.remove(index);
    }

    pub fn connect(&mut self, to_id: usize, in_pin: Pin, from_id: usize) {
        let my_index = self.comps[from_id].out.len();
        self.comps[from_id].out.push(SimOutConn::new(to_id, in_pin));

        let wires = vec![];
        match in_pin {
            Pin::A => {
                self.comps[to_id].a = Some(SimInConn::new(
                    from_id, my_index, wires));
            }
            Pin::B => {
                self.comps[to_id].b = Some(SimInConn::new(
                    from_id, my_index, wires));
            }
        }
    }

    pub fn eval_all(&mut self, eval_count: usize) {
        for i in 0..self.comps.len() {
            self.eval_one(i, eval_count);
        }
    }

    pub fn eval_one(&mut self, comp_index: usize, eval_count: usize) {
        let comp = &mut self.comps[comp_index];
        if comp.eval_count > eval_count {
            return;
        }

        comp.eval_count += 1;

        if let Some(SimInConn {
            parent_id: a_index, ..
        }) = self.comps[comp_index].a {
                self.eval_one(a_index, eval_count);
            };
        if let Some(SimInConn {
            parent_id: b_index, ..
        }) = self.comps[comp_index].b {
                self.eval_one(b_index, eval_count);
            };

        let comp = &mut self.comps[comp_index];
        if comp.out.len() == 0 {
            return;
        }

        let a;
        if let Some(SimInConn { value, .. })
            = comp.a {
                a = value;
            } else { a = false; }

        let b;
        if let Some(SimInConn { value, .. })
            = comp.b {
                b = value;
            } else { b = false; }

        let out: bool;
        match comp.kind {
            SimCompKind::Buffer => out = a,
            SimCompKind::Not => out = !a,
            SimCompKind::Or => out = a | b,
            SimCompKind::And => out = a & b,
        }

        for i in 0..comp.out.len() {
            let child_id = self.comps[comp_index].out[i].comp_id;
            match self.comps[comp_index].out[i].pin {
                Pin::A => self.comps[child_id].a
                    .as_mut().unwrap().value = out,
                Pin::B => self.comps[child_id].b
                    .as_mut().unwrap().value = out,
            }
        }
    }

    pub fn set_comp_position(
        &mut self, comp_index: usize,
        new_pos: Vector2, gs: f32
    ) {
        let comp = &mut self.comps[comp_index];
        comp.position = new_pos;

        let out_pin_pos = comp.out_pin_pos(gs);
        if comp.a.is_some() {
            comp.a.as_mut().unwrap().wires
                .first_mut().unwrap().start = comp.a_pin_pos(gs);
            comp.a.as_mut().unwrap().wires
                .last_mut().unwrap().end = out_pin_pos;
        }

        if comp.b.is_some() {
            comp.b.as_mut().unwrap().wires
                .first_mut().unwrap().start = comp.b_pin_pos(gs);
            comp.b.as_mut().unwrap().wires
                .last_mut().unwrap().end = out_pin_pos;
        }

        for i in 0..comp.out.len() {
            // let child = &mut comp.out[i];
            let a = self.comps[comp_index].out[i].comp_id;

            match self.comps[comp_index].out[i].pin {
                Pin::A => self.comps[a].a.as_mut().unwrap()
                    .wires.last_mut().unwrap().end = out_pin_pos,
                Pin::B => todo!(),
            }
        }
    }
}
