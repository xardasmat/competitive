use std::collections::HashMap;
use std::collections::VecDeque;

#[derive(Debug)]
struct Graph {
    e : Vec<HashMap<usize, i32>>,
}

impl Graph {

    fn new(n: usize) -> Self {
        Self {
            e: vec![HashMap::new(); n],
        }
    }

    fn add_edge(&mut self, a: usize, b: usize, cap: i32) {
        *self.e[a].entry(b).or_insert(0) += cap;
    }

    fn add_path(&mut self, s: usize, t: usize) -> bool {
        let mut q = VecDeque::<usize>::new();
        let mut ret = vec![self.e.len(); self.e.len()];
        let mut visited = vec![false; self.e.len()];
        visited[s] = true;
        q.push_back(s);

        while let Some(u) = q.pop_front() {
            for (v, c) in &self.e[u] {
                if visited[*v] || *c <= 0 { continue; }
                ret[*v] = u;
                q.push_back(*v);
                visited[*v] = true;
            }
        }
        if visited[t] == false { return false; }
        let mut u = t;
        while u != s {
            let v = ret[u];
            self.add_edge(u, v, 1);
            self.add_edge(v, u, -1);
            u = v;
        }
        return true;
    }
}

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let h: usize = first_line.next().unwrap().parse().unwrap();
    let w: usize = first_line.next().unwrap().parse().unwrap();

    let mut map = vec![String::new(); h];
    for i in 0..h {
        std::io::stdin().read_line(&mut map[i]).unwrap();
    }
    
    let mut g : Graph = Graph::new(h*w+2);
    let s = h*w;
    let t = h*w+1;
    let inf = 1000000000;
    let mut pluses = 0;
    
    for i in 0..h {
        for j in 0..w {
            let v = i*w+j;
            match map[i].chars().nth(j).unwrap() {
                '+' => {g.add_edge(s, v, 1); pluses += 1},
                '-' => g.add_edge(v, t, 1),
                _ => (),
            }
            if map[i].chars().nth(j).unwrap() == '#' { continue; }
            if j+1 < w && map[i].chars().nth(j+1).unwrap() != '#' {
                let u = v+1;
                g.add_edge(u, v, inf);
                g.add_edge(v, u, inf);
            }
            if i+1 < h && map[i+1].chars().nth(j).unwrap() != '#' {
                let u = v+w;
                g.add_edge(u, v, inf);
            }
        }
    }

    let mut total = 0;
    while g.add_path(s, t) {
        total += 1;
    }
    println!("{}", pluses - total);
}