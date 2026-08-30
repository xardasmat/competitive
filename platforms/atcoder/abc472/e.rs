#[derive(Debug)]
struct G {
    E : Vec<Vec<usize>>,
    C : Vec<bool>,
    P : Vec<usize>,
}

impl G {

    fn new(n: usize) -> Self {
        Self {
            E: vec![Vec::new(); n],
            C: vec![false; n],
            P: vec![n; n],
        }
    }

    fn add_edge(&mut self, a: usize, b: usize) {
        self.E[a].push(b);
        self.E[b].push(a);
    }

    fn dfs(&mut self, v: usize, c: bool) {
        for wi in 0..self.E[v].len() {
            let w = self.E[v][wi];
            if self.P[w] < self.E.len() { continue; }
            self.C[w] = !c;
            self.P[w] = v;
            self.dfs(w, !c);
        }
    }

    fn track(&self, v : usize) -> Vec<usize> {
        let mut path = vec![v];
        let mut vv = v;
        while vv > 0 {
            vv = self.P[vv];
            path.push(vv);
        }
        return path;
    }

    fn find(&self) -> Option<Vec<usize>> {
        for v in 0..self.E.len() {
            for wi in 0..self.E[v].len() {
                let w = self.E[v][wi];
                if self.C[v] != self.C[w] { continue; }
                let mut A = self.track(v);
                let mut B = self.track(w);
                let mut last = self.E.len();
                while A.last() == B.last() {
                    last = *A.last().unwrap();
                    A.pop();
                    B.pop();
                }
                A.reverse();
                A.push(last);
                return Some([A.as_slice(), B.as_slice()].concat());
            }
        }
        return None;
    }
}

fn solve() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let N: i32 = first_line.next().unwrap().parse().unwrap();
    let M: i32 = first_line.next().unwrap().parse().unwrap();

    let mut g = G::new(N as usize);

    for i in 0..M {
        let mut input = String::new();
        std::io::stdin().read_line(&mut input).unwrap();
        let mut first_line = input.split_whitespace();
        let a: i32 = first_line.next().unwrap().parse().unwrap();
        let b: i32 = first_line.next().unwrap().parse().unwrap();
        g.add_edge((a-1) as usize, (b-1) as usize);
    }
    // dbg!(&g);
    for v in 0..N {
        if g.P[v as usize] < (N as usize) { continue; }
        g.P[v as usize] = v as usize;
        g.dfs(v as usize, false);
    }
    match g.find() {
        Some(path) => println!("{}\n{}", path.len(), path.iter().map(|n| (n+1).to_string()).collect::<Vec<String>>().join(" ")),
        None => println!("-1"),
    }

}

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let T: i32 = first_line.next().unwrap().parse().unwrap();
    
    for t in 0..T {
        solve();
    }
}