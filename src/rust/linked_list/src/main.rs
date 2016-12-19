use std::fmt;
use std::option;
struct Node {
    data : i32,
    next : Option<Box<Node>>,
}


impl Node {
    fn append(&self, content: i32) {
        Node{data: content, next: None};
    }
}

impl fmt::Display for Node {
    fn fmt(&self, f: & mut fmt::Formatter) -> fmt::Result {
        let current = format!("({})", self.data);
        let others =
          match self.next {
            None => format!("."),
            Some(ref next_data) => format!(" --> {}", next_data)
          };
        write!(f, "{}{}", current, others)
    }
}

fn main() {
    let n1 = Node{data: 0, next: None};
    let n2 = Node{data: 1, next: Some(Box::new(n1))};

    println!("Result: {}", n2);
}

