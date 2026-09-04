
fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let n: usize = input.split_whitespace().next().unwrap().parse().unwrap();

    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let a : Vec<i32> = input.split_whitespace().map(|word| word.parse::<i32>().unwrap()).collect();
    let sum : i32 =  a[n/2..].iter().sum();
    println!("{}", sum);
}