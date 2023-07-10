use raylib::prelude::*;

pub enum SimCompKind {
    Buffer,
    Not,
    Or,
    And,
}

impl SimCompKind {
    pub fn get_text(&self) -> &str {
        match *self {
            Self::Buffer => "Buffer",
            Self::Not => "Not",
            Self::Or => "Or",
            Self::And => "And",
        }
    }

    pub fn is_binary(&self) -> bool {
        match *self {
            Self::Buffer => false,
            Self::Not => false,
            Self::Or => true,
            Self::And => true,
        }
    }
}

pub struct Wire {
    pub start: Vector2,
    pub end: Vector2,
}

pub struct SimInConn {
    pub value: bool,
    pub parent_id: usize,
    pub my_index: usize,
    pub wires: Vec<Wire>,
}

impl SimInConn {
    pub fn new(parent_id: usize, my_index: usize, wires: Vec<Wire>) -> Self {
        Self {
            value: false,
            parent_id,
            my_index,
            wires,
        }
    }
}

#[derive(Clone, Copy)]
pub enum Pin {
    A,
    B,
}

#[derive(Clone, Copy)]
pub struct SimOutConn {
    pub comp_id: usize,
    pub pin: Pin,
}

impl SimOutConn {
    pub fn new(comp_id: usize, pin: Pin) -> Self {
        Self {
            comp_id, pin
       }
    }
}

pub struct SimComp {
    pub position: Vector2,
    pub kind: SimCompKind,
    pub a: Option<SimInConn>,
    pub b: Option<SimInConn>,
    pub out: Vec<SimOutConn>,
    pub eval_count: usize,
}

impl SimComp {
    pub fn new(kind: SimCompKind, position: Vector2) -> Self {
        Self {
            position,
            kind,
            a: None,
            b: None,
            out: vec![],
            eval_count: 0,
        }
    }
}

pub struct SimComps {
    pub comps: Vec<SimComp>,
}

impl SimComps {
    pub fn new() -> Self {
        Self {
            comps: vec![],
        }
    }
}
