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

    while !rh.window_should_close() {
        global_state.screen_width = rh.get_screen_width() as u32;
        global_state.screen_height = rh.get_screen_height() as u32;

        let mut canvas = rh.begin_drawing(&thread);
        canvas.clear_background(Color::BLACK);
        canvas.draw_fps(100, 100);
    }
}
