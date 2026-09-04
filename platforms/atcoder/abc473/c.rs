
fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let _n: usize = input.split_whitespace().next().unwrap().parse().unwrap();
    let k: usize = input.split_whitespace().next().unwrap().parse().unwrap();

    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let a : Vec<usize> = input.split_whitespace().map(|word| word.parse::<usize>().unwrap()).collect();
    
    let mut v = vec![0; k];
    for s in a {
        v[s-1] += 1;
    }
    let top = *v.iter().max().unwrap();
    let tops = v.iter().filter(|&&c| c == top).count();
    let topless = v.iter().filter(|&&c| c == top-1).count();
    let sum = tops+topless;
    println!("{}", sum);
}
