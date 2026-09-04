


fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let _n: usize = iter.next().unwrap().parse().unwrap();
    let k: usize = iter.next().unwrap().parse().unwrap();
    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let a : Vec<usize> = input.split_whitespace().map(|word| word.parse::<usize>().unwrap()).collect();

    let mut count: usize = 0;
    let mut sum: usize = 0;
    let mut hs = std::collections::HashSet::<usize>::new();
    for v in &a {
        sum = (sum+v)%k;
        if hs.contains(&sum) || sum == 0 {
            hs.clear();
            count += 1;
            sum = 0;
        }
        else {hs.insert(sum);}
    }

    println!("{}", count);
}
