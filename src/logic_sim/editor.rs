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
