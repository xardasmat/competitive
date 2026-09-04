
fn f(i: i32, n: i32, k: i32) -> Vec::<Vec::<i32>> {
    // dbg!("xd", i, n, k);
    let mut v = Vec::<Vec::<i32>>::new();
    if i == n {
        if k/i*i == k {
            v.push(vec![k/i]);
        }
        return v;
    }
    for ss in 0..=k/i {
        for mut vv in &mut f(i+1, n, k-ss*i) {
            let mut vvv = vec![ss];
            vvv.append(&mut vv);
            v.push(vvv);
        }
    }
    return v;
}

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let n: i32 = iter.next().unwrap().parse().unwrap();
    let k: i32 = iter.next().unwrap().parse().unwrap();

    let v = f(1, n, k);
    for vv in v {
        println!("{} ", vv
        .iter()
        .map(|x| x.to_string())
        .collect::<Vec<String>>()
        .join(" "));
    }
}
