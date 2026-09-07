
fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let n: usize = first_line.next().unwrap().parse().unwrap();
    let q: usize = first_line.next().unwrap().parse().unwrap();
    
    let mut t = vec![0; n+1];
    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut p : Vec<usize> = input.split_whitespace().map(|word| word.parse::<usize>().unwrap()).collect();
    for i in 0..n { t[p[i]] = i; }

    for _l in 0..q {
        let mut input = String::new();
        std::io::stdin().read_line(&mut input).unwrap();
        let mut first_line = input.split_whitespace();
        let a: usize = first_line.next().unwrap().parse::<usize>().unwrap();

        p.push(a);
        p[t[a]] = 0;
        t[a] = p.len()-1;
    }
    for v in p {
        if v != 0 { print!("{} ", v) }
    }
    println!();
}