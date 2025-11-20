use crate::types::Corners::{BL, BR, TL, TR};

pub struct Params {
    pub width: u32,
    pub height: u32,
    pub channels: u8
}

#[derive(Debug)]
pub struct Color24 {
    pub red: u8,
    pub green: u8,
    pub blue: u8,
}

#[repr(usize)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Corners {
    TL = 0,     // Top left corner
    TR,     // Top right corner
    BL, // Bottom left corner
    BR,     // Bottom right corner
}


pub struct BackgroundColor {
    corners: Vec<Color24>,
}

impl BackgroundColor {
    pub fn new(corners: Vec<Color24>) -> BackgroundColor {
        assert_eq!(corners.len(), 4, "Bad initialization of Corners");
        BackgroundColor{corners}
    }

    fn lerp(a: &Color24, b: &Color24, t: f64) -> Color24{
        let red: u8 = ((1.0 - t)* a.red as f64 + t * b.red as f64) as u8;
        let green: u8 = ((1.0 - t)* a.green as f64 + t * b.green as f64) as u8;
        let blue: u8 = ((1.0 - t)* a.blue as f64 + t * b.blue as f64) as u8;

        Color24{red, green, blue}
    }

    pub fn sample_uv(&self, u: f64, v: f64) -> Color24 {
        assert_eq!(self.corners.len(), 4, "Bad initialization of Corners");

        let xb: Color24 = Self::lerp(&self.corners[BL as usize], &self.corners[BR as usize], u);
        let xt: Color24 = Self::lerp(&self.corners[TL as usize], &self.corners[TR as usize], u);

        Self::lerp(&xt, &xb, v)
    }
}