use std::collections::HashSet;
use std::collections::VecDeque;

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let h: i32 = first_line.next().unwrap().parse().unwrap();
    let w: i32 = first_line.next().unwrap().parse().unwrap();
    let k: i32 = first_line.next().unwrap().parse().unwrap();


    let mut rows = HashSet::new();
    let mut cols = HashSet::new();
    let mut M : Vec<String> = Vec::new();
    for j in 0..h {
        input = String::new();
        std::io::stdin().read_line(&mut input).unwrap();
        M.push(input);
        for i in 0..w {
            if M[j as usize].as_bytes()[i as usize] == b'#' {
                rows.insert(j);
                cols.insert(i);
            }
        }
    }
    
    let mut D : Vec<Vec<i32>> = vec![vec![k+1; w as usize]; h as usize];
    let mut Q = VecDeque::new();
    for j in 0..h {
        if rows.contains(&j) {continue;}
        for i in 0..w {
            if cols.contains(&i) {continue;}
            Q.push_back( (i,j) );
            D[j as usize][i as usize] = 0;
        }
    }
    let mut result = Q.len();
    while let Some((i, j)) = Q.pop_front() {
        for (di, dj) in [(0, 1), (0, -1), (1, 0), (-1, 0)] {
            let ni = i+di;
            let nj = j+dj;
            // out of bounds
            if ni < 0 || ni >= w || nj < 0 || nj >= h { continue; }
            // already visited
            if D[nj as usize][ni as usize] <= k { continue; }
            // bomb
            if M[nj as usize].as_bytes()[ni as usize] == b'#' { continue; }
            D[nj as usize][ni as usize] = D[j as usize][i as usize] + 1;
            if D[nj as usize][ni as usize] <= k {
                result += 1;
            }
            if D[nj as usize][ni as usize] < k {
                Q.push_back((ni, nj));
            }
        }
    }
    println!("{}", result);
}