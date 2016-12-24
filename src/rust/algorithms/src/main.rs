mod binary_search;
mod point_2d;
fn main() {
    let (index, error) = binary_search::index_of(vec![1, 2, 3, 4, 5], 3);
    println!("index {}, error {}", index, error);
    
    
    let point = point_2d::Point2D{x : 3.0, y : 4.0};
    println!("point radius: {}", point.r());
}
