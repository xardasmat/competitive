use std::cmp::min;

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let n: i32 = input.split_whitespace().next().unwrap().parse().unwrap();

    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let L : Vec<i32> = input.split_whitespace().map(|word| word.parse::<i32>().unwrap()).collect();
    let mut rsum : i32 = L.iter().sum();
    let mut lsum : i32 = 0;
    let mut best = rsum;
    for l in L {
         rsum -= l;
         lsum += l;
         best = min(best, (rsum - lsum).abs());
    }
    println!("{}", best);

}