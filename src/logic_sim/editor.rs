use raylib::color::Color;

pub const BACKGROUND_COLOR:     Color = Color::new(0x16, 0x18, 0x1f, 0xFF);
pub const TITLE_COLOR:          Color = Color::new(0xF9, 0xB9, 0xAB, 0xFF);
pub const GATE_BOX_COLOR:       Color = Color::new(0x4C, 0x7C, 0x94, 0xFF);
pub const GATE_LABLE_COLOR:     Color = Color::new(0xEC, 0x84, 0x94, 0xFF);
pub const INDICATOR_ON_COLOR:   Color = Color::new(0xC4, 0xDC, 0xE6, 0xFF);
pub const INDICATOR_OFF_COLOR:  Color = Color::new(0x67, 0x3D, 0x2C, 0xFF);

const PADDING: f32 = 2.;
const LABLE_SIZE: f32 = 6.;
const JOINT_RADIUS: f32 = 1.;
const BOX_THICKNESS: f32 = 0.5;
const KNIFE_THICKNESS: f32 = 0.5;

#[inline]
pub fn padding(gs: f32) -> f32 {
    gs * PADDING
}

#[inline]
pub fn lable_size(gs: f32) -> f32 {
    gs * LABLE_SIZE
}

#[inline]
pub fn joint_radius(gs: f32) -> f32 {
    gs * JOINT_RADIUS
}

#[inline]
pub fn box_thickness(gs: f32) -> f32 {
    gs * BOX_THICKNESS
}

#[inline]
pub fn knife_thickness(gs: f32) -> f32 {
    gs * KNIFE_THICKNESS
}
