fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let n: i32 = first_line.next().unwrap().parse().unwrap();
    let m: i32 = first_line.next().unwrap().parse().unwrap();
    let k: i64 = first_line.next().unwrap().parse().unwrap();

    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut A : Vec<i64> = input.split_whitespace().map(|word| word.parse().unwrap()).collect();
    let mut tail : i64 = 0;
    for i in 0..n {
        let eat = tail + A[i as usize] <= k;
        if eat {
            tail += A[i as usize];
        } else {
            A[i as usize] = 0;
        }
        if i-m+1 >= 0 {
            tail -= A[(i-m+1) as usize];
        }
        println!("{}", match eat {
            true => "Yes",
            false => "No",
            });
    }
}