use raylib::prelude::*;

pub mod models;
pub mod logic_sim;

use models::*;

fn main() {
    let mut global_state = GlobalState::new(1920, 1080, 10.);
    let (mut rh, thread) = init()
        .size(global_state.screen_width as i32,
            global_state.screen_height as i32)
        .title("Nor2doom")
        .resizable()
        .vsync()
        .build();

    let mut logic_simulator = logic_sim::LogicSim::new();
    logic_simulator.add_new(logic_sim::SimCompKind::Buffer, Vector2::new( 300., 300. ));
    logic_simulator.add_new(logic_sim::SimCompKind::Buffer, Vector2::new( 300., 500. ));
    logic_simulator.add_new(logic_sim::SimCompKind::Not,    Vector2::new( 550., 300. ));
    logic_simulator.add_new(logic_sim::SimCompKind::Not,    Vector2::new( 550., 500. )); 
    logic_simulator.add_new(logic_sim::SimCompKind::Not,    Vector2::new( 800., 200. ));
    logic_simulator.add_new(logic_sim::SimCompKind::And,    Vector2::new( 800., 600. ));
    logic_simulator.add_new(logic_sim::SimCompKind::And,    Vector2::new( 1000., 400. ));
    logic_simulator.add_new(logic_sim::SimCompKind::Or,     Vector2::new( 1200., 400. ));

    // TODO: Use smart pointer instead of mutable reference to make it possible
    // logic_simulator.connect(&mut logic_simulator.comps[2], logic_sim::Pin::A, &mut logic_simulator.comps[3]);

    let mut eval_count = 0;

    while !rh.window_should_close() {
        global_state.screen_width = rh.get_screen_width() as u32;
        global_state.screen_height = rh.get_screen_height() as u32;

        let mut canvas = rh.begin_drawing(&thread);
        canvas.clear_background(Color::BLACK);

        logic_simulator.update(&mut canvas, eval_count, &global_state);

        canvas.draw_fps(100, 100);
        eval_count += 1;
    }
}
