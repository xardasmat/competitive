
fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let _n: usize = input.split_whitespace().next().unwrap().parse().unwrap();

    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let ok  = input.
        split_whitespace().
        map(|word| word.parse::<usize>().unwrap()).
        enumerate().
        all(|(i, n)| i/10 == (n-1)/10);
    match ok {
        true => println!("Yes"),
        false => println!("No"),
    }
}