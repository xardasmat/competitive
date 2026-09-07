
fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut n: usize = input.split_whitespace().next().unwrap().parse().unwrap();

    n += 1;
    if n > 3 { n = 1; }
    println!("{}", n);
}