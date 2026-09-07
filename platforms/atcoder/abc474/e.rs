
struct A {
    a : usize,
    b : usize,
}

fn solve() {
    
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let n: usize = first_line.next().unwrap().parse().unwrap();

    let mut a : Vec<A> = vec![];
    let mut cheapest = 1000000000;

    for _i in 0..n {
        let mut input = String::new();
        std::io::stdin().read_line(&mut input).unwrap();
        let mut line = input.split_whitespace();
        let aa: usize = line.next().unwrap().parse().unwrap();
        let bb: usize = line.next().unwrap().parse().unwrap();
        a.push(A{a:aa,b:bb});
        cheapest = std::cmp::min(cheapest, aa);
    }


    let mut total = 0;
    a.sort_by(|x, y| ( (x.a-x.b, -(x.a as i32)) ).cmp(&((y.a-y.b, -(y.a as i32)))));
    for x in &a {
        total += x.a;
    }
    let mut best = total;
    
    let half = n/2;
    for (i, x) in a.into_iter().rev().enumerate() {
        total -= x.a;
        total += x.b;
        if n%2 == 1 && i == half {
            total += cheapest;
        } else if i >= half {
            total += 2*cheapest;
        }
        best = std::cmp::min(best, total);
    }
    println!("{}", best);
}

fn main() {
    
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let t: usize = first_line.next().unwrap().parse().unwrap();
    for _i in 0..t {
        solve();
    }
}