/**
 * Simple algorithm for binary search in a vector.
 * 
 * Code based on this one: 
 * http://algs4.cs.princeton.edu/11model/BinarySearch.java.html
 */

pub fn index_of(input_vector: Vec<i32>, value: i32) -> (usize, i8) {
    println!("Running binary search algorithm.");
    
    let mut lo : usize = 0;
    let mut hi = input_vector.len() - 1;
    
    while lo <= hi {
        let mid = lo + (hi - lo) / 2;
        if value < input_vector[mid] {
            hi = mid - 1;
        } else if value > input_vector[mid] {
            lo = mid + 1;
        } else {
            return (mid, 0)
        }
    }         
    (0, -1)
}
