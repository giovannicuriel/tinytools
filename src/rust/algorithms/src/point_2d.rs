/**
 * Reimplementation of this file
 * http://algs4.cs.princeton.edu/12oop/Point2D.java.html
 * 
 * in Rust
 */
pub struct Point2D {
    pub x : f32,
    pub y : f32
}

impl Point2D {
    pub fn r(&self) -> f32 {
        (self.x.powf(2.0) + self.y.powf(2.0)).sqrt()
    }
}

