// TASK:
// 1. Implement the increase_volume and decrease_volume functions so that they increase or decrease the volume by 5%, passing the tests.
// 2. Implement the main function

use std::cmp::{max, min};

fn increase_volume(volume: i32) -> i32 {
    return min(volume + 5, 100);
}

fn decrease_volume(volume: i32) -> i32 {
    return max(volume - 5, 0);
}

#[allow(dead_code)]
#[allow(unused_variables)]
#[warn(unused_assignments)]
fn main() {
    let mut var = 4;
    let ref_var: &i32 = &var;

    println!("{}", var);
    println!("{}", *ref_var);

    var = 5; // не скомпилируется
    // *ref_var = 6; // и это

    println!("{}", var);

    // Изменяемые ссылки
    //
    let mut var2 = 4;
    let ref_var2: &mut i32 = &mut var2;
    *ref_var2 += 2;

    println!("{var2}");

    let mut tpl1: &str = "{var2}";
    println!("{tpl1}");
}