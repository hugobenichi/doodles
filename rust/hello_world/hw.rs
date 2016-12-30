/* programming in rust */
#![allow(dead_code)]
#![allow(unused_variables)]

use std::fmt;
use std::io;

#[derive(Debug)]
struct S(i32);

impl fmt::Display for S {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "S(#0:{})", self.0)
    }
}

#[derive(Debug)]
struct Point2 {
    x: f64,
    y: f64,
}

#[derive(Debug)]
struct Point3 {
    x: f64,
    y: f64,
    z: f64,
}

impl fmt::Display for Point3 {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "({},{},{})", self.x, self.y, self.z)
    }
}

fn readline() -> String {
    let mut line = String::new();
    io::stdin()
        .read_line(&mut line)
        .ok()
        .expect("Failed to read line!");
    return String::from(line.trim());
}

#[derive(Debug)]
struct Info2 {
    name:       String,
    age:        String,
    username:   String,
}

fn test() {
    let inf = Info2 {
        name:     readline(),
        age:      readline(),
        username: readline(),
    };

    println!("{:?}", inf);
}

// my main function
fn main() {

    test();

    let x = 5 + 4 * 2;

    println!("hw ! x = {}", x);

    #[allow(unused_variables)]
    let y = 42 - 10;

    println!("{arg2} {another_arg} {bla}", arg2 = "foo", another_arg = "bar", bla = "baz");

    let f = "a string";
    println!("x = {} ({:?})", f, f);

    println!("x = {} ({:?})", x, x);

    let s = S(32);
    println!("s = {} ({:?})", s, s);

    let p = Point3{x: 6.0, y: 3.0, z: -1.0};
    println!("p = {} ({:?})", p, p);
}
