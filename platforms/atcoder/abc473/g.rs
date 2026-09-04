// fn split(v : &Vec<i64>) -> (Vec<i64>,Vec<i64>) {
//     let mut even : Vec::<i64> = vec![];
//     let mut odd : Vec::<i64> = vec![];
//     for (i, e) in v.iter().enumerate() {
//         if i % 2 == 0 {
//             even.push(*e);
//         } else {
//             odd.push(*e);
//         }
//     }
//     (even, odd)
// }

// fn fft(f : &Vec<i64>, root : i64, m : i64) -> Vec<i64> {
//     let n = f.len();
//     if n == 1 { return f.clone(); }
//     let (l, r) = split(f);
//     let even = fft(&l, root*root%m, m);
//     let odd = fft(&r, root*root%m, m);
//     let mut ff : Vec::<i64> = vec![0; n];
//     let mut omega = 1;
//     for i in 0..n/2 {
//         ff[i] = (even[i] + omega*odd[i]%m) % m;
//         ff[i+n/2] = (even[i] - omega*odd[i]%m + m) % m;
//         omega *= root;
//         omega %= m;
//     }
//     ff
// }

fn fft2(f : &mut[i64], r : &mut [i64], n : usize, root : i64, m : i64) {
    if n == 1 { return; }
    for i in 0..n/2 {
        r[i+n/2] = f[2*i+1];
    }
    for i in 0..n/2 {
        r[i] = f[2*i];
    }
    fft2(&mut r[0..n/2], &mut f[0..n/2], n/2, root*root%m, m);
    fft2(&mut r[n/2..n], &mut f[n/2..n], n/2, root*root%m, m);
    
    let mut omega = 1;
    for i in 0..n/2 {
        r[i] = (f[i] + omega*f[i+n/2]%m) % m;
        r[i+n/2] = (f[i] - omega*f[i+n/2]%m + m) % m;
        omega *= root;
        omega %= m;
    }
}

fn gcd(a : i64, b : i64) -> (i64, i64, i64) {
    if b == 0 { return (1, 0, a); }
    let (x, y, d) = gcd(b, a%b);
    (y, x-a/b*y, d)
}

fn inv(a : i64, m : i64) -> i64 {
    let (x, _y, _d) = gcd(a, m);
    (m+x%m)%m
}

fn convolute(mut a : Vec<i64>, mut b : Vec<i64>) -> Vec<i64> {
    let m = 998244353;
    let mut root = 31;
    let mut n = 1<<23;
    let nn = a.len() + b.len() - 1;
    while n/2 > nn {
        root *= root;
        root %= m;
        n/=2;
    }
    while a.len() < n { a.push(0); }
    while b.len() < n { b.push(0); }

    // let mut aa = fft(&a, root, m);
    // let bb = fft(&b, root, m);
    let mut aa : Vec<i64> = vec![0_i64; n];
    fft2(&mut a[0..n], &mut aa[0..n], n, root, m);
    fft2(&mut b[0..n], &mut a[0..n], n, root, m);
    
    for i in 0..n {
        aa[i] *= a[i];
        aa[i] %= m;
    }
    fft2(&mut aa[0..n], &mut a[0..n], n, inv(root, m), m);
    
    let inv_n = inv(n as i64, m);
    for i in 0..n {
        a[i] *= inv_n;
        a[i] %= m;
    }
    // while a.len() > 1 && *a.last().unwrap() == 0 {
    //     a.pop();
    // }
    a
}

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let n: usize = iter.next().unwrap().parse().unwrap();
    let k: usize = iter.next().unwrap().parse().unwrap();
    
    if k >= 2*n {
        println!("0");
        return;
    }
    let mut q = std::collections::VecDeque::<Vec<i64>>::new();
    for i in 1..=n {
        let v = vec![1_i64,(i-1) as i64];
        q.push_back(v);
    }
    while q.len() >= 2 {
        let a = q.pop_front().unwrap();
        let b = q.pop_front().unwrap();
        let c = convolute(a, b);
        q.push_back(c);
    }
    let m = 998244353_i64;
    let mut fac = 1_i64;
    for i in 1..=n {
        fac *= i as i64;
        fac %= m;
    }
    let c = q.pop_front().unwrap();
    if k-n >= c.len() {
        println!("0");
    } else {
        println!("{}", c[k-n] * inv(fac, m) % m);
    }
}
