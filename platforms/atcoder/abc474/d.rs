
fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let n: usize = first_line.next().unwrap().parse().unwrap();
    
    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let a : Vec<usize> = input.split_whitespace().map(|word| word.parse::<usize>().unwrap()).collect();
    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let b : Vec<usize> = input.split_whitespace().map(|word| word.parse::<usize>().unwrap()).collect();

    let mut ok = false;
    for i in 0..n {
        ok |= a[i] > b[i];
    }
    if !ok {
        println!("No");
        return;
    }
    println!("Yes");
    for i in 0..n {
        if a[i] > b[i] { print!("{} ", (n as i64) * 1000000000_i64); }
        else { print!("1 "); }
    }
    println!();
}