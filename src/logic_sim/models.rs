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

pub struct SimInConn<'a> {
    pub value: bool,
    pub source: Option<(&'a mut SimComp<'a>, usize)>, // own index in parent's out list
    pub wires: Vec<Wire>,
}

impl SimInConn<'_> {
    pub fn new() -> Self {
        Self {
            value: false,
            source: None,
            wires: vec![],
        }
    }
}

#[derive(Clone, Copy)]
pub enum Pin {
    A,
    B,
}

pub struct SimOutConn<'a> {
    pub comp: &'a mut SimComp<'a>,
    pub pin: Pin,
}

impl<'a> SimOutConn<'a> {
    pub fn new(comp: &'a mut SimComp<'a>, pin: Pin) -> Self {
        Self {
            comp,
            pin
        }
    }
}

pub struct SimComp<'a> {
    pub position: Vector2,
    pub kind: SimCompKind,
    pub a: SimInConn<'a>,
    pub b: SimInConn<'a>,
    pub out: Vec<SimOutConn<'a>>,
    pub eval_count: usize,  // How many times it has been evaluated before
                            // To sync all the components
}

impl SimComp<'_> {
    pub fn new(kind: SimCompKind, position: Vector2) -> Self {
        Self {
            position,
            kind,
            a: SimInConn::new(),
            b: SimInConn::new(),
            out: vec![],
            eval_count: 0,
        }
    }
}

pub struct LogicSim<'a> {
    pub comps: Vec<SimComp<'a>>,
}

impl LogicSim<'_> {
    pub fn new() -> Self {
        Self {
            comps: vec![],
        }
    }
}
