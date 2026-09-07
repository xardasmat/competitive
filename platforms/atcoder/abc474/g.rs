

fn solve() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let n: usize = first_line.next().unwrap().parse().unwrap();
    let k: usize = first_line.next().unwrap().parse().unwrap();

    
}

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let t: usize = first_line.next().unwrap().parse().unwrap();
    for _i in 0..t {
        solve();
    }
}