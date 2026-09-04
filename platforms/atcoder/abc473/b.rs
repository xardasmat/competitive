
fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let _n: usize = input.split_whitespace().next().unwrap().parse().unwrap();

    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let a : Vec<i32> = input.split_whitespace().map(|word| word.parse::<i32>().unwrap()).collect();
    
    let mut hm = std::collections::HashMap::new();
    for c in a {
        *hm.entry(c).or_insert(0) += 1;
    }
    let mut sum = 0;
    for (k, v) in hm.iter() {
        if v % 2 == 1 {
            sum += k;
        }
    }
    println!("{}", sum);
}