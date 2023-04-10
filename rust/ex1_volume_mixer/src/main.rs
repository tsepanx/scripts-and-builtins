// TASK:
// 1. Implement the increase_volume and decrease_volume functions so that they increase or decrease the volume by 5%, passing the tests.
// 2. Implement the main function

use std::cmp::{max, min};
use std::io;

fn increase_volume(volume: i32) -> i32 {
    return min(volume + 5, 100);
}

fn decrease_volume(volume: i32) -> i32 {
    return max(volume - 5, 0);
}

fn main() {
    let term = console::Term::stdout();

    let mut volume = 50;

    let a: Option<i32> = Some(333);

    match a {
        Some(x) => println!("Some: {}", x),
        None        => println!("None")
    }

    loop {
        println!("What to do? [i/d]?");
        let c = term.read_char().expect("Failed to read input");
        println!("You pressed: {}", c);

        if c == 'i' {
            volume = increase_volume(volume);
        } else if c == 'd' {
            volume = decrease_volume(volume);
        }
        println!("Current volume {}", volume);
    }


    // todo!("Loop indefinitely, read a character specifying whether the volume should be increased or decreased and print the new volume");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn increase_volume_test() {
        assert_eq!(increase_volume(50), 55);
    }

    #[test]
    fn decrease_volume_test() {
        assert_eq!(decrease_volume(50), 45);
    }

    #[test]
    fn increase_volume_test_overflow() {
        assert_eq!(increase_volume(100), 100);
    }

    #[test]
    fn decrease_volume_test_underflow() {
        assert_eq!(decrease_volume(0), 0);
    }
}