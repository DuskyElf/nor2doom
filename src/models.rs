pub struct GlobalState {
    pub screen_width: u32,
    pub screen_height: u32,
    pub gui_scale: f32,
}

impl GlobalState {
    pub fn new(
        screen_width: u32,
        screen_height: u32,
        gui_scale: f32
    ) -> Self {
        Self {
            screen_width,
            screen_height,
            gui_scale
        }
    }
}
