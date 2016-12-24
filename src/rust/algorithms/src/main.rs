mod binary_search;
mod point_2d;
fn main() {
    let (index, error) = binary_search::index_of(vec![1, 2, 3, 4, 5], 3);
    println!("index {}, error {}", index, error);
}
