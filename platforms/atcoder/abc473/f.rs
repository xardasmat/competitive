
#[derive(Debug)]
struct RangeTree {
    v: Vec<i32>,
    m: Vec<i32>,
    size : usize,
}

impl RangeTree {
    fn new(size: usize) -> Self {
        Self {
            v : vec![0; 2*size],
            m : vec![0; 2*size],
            size: size,
        }
    }

    #[allow(dead_code)]
    fn debug(&self) {
        let mut s = self.size;
        while s > 0 {
            for i in s..2*s {
                print!("{}_{} ", self.v[i], self.m[i]);
            }
            println!();
            s /= 2;
        }
    }

    fn add(&mut self, l:usize, r:usize, v:i32) {
        let mut li = l+self.size;
        let mut ri = r+self.size;
        self.v[li] += v;
        if li != ri {
            self.v[ri] += v;
        }
        while li/2 < ri/2 {
            if li % 2 == 0 {
                self.v[li+1] += v;
            }
            if ri % 2 == 1 {
                self.v[ri-1] += v;
            }
            li /= 2;
            ri /= 2;
            
            self.m[li] = std::cmp::min(
                self.m[2*li] + self.v[2*li],
                self.m[2*li+1] + self.v[2*li+1]);
            self.m[ri] = std::cmp::min(
                self.m[2*ri] + self.v[2*ri],
                self.m[2*ri+1] + self.v[2*ri+1]);
        }
        
        while li > 1 {
            li /= 2;
            self.m[li] = std::cmp::min(
                self.m[2*li] + self.v[2*li],
                self.m[2*li+1] + self.v[2*li+1]);
        }
    }

    fn get(&self, l:usize, r:usize) -> i32 {
        let mut li = l+self.size;
        let mut ri = r+self.size;
        let mut lmin = self.v[li];
        let mut rmin = self.v[ri];
        while li/2 < ri/2 {
            if li % 2 == 0 {
                lmin = std::cmp::min(lmin, self.m[li+1]+self.v[li+1]);
            }
            if ri % 2 == 1 {
                rmin = std::cmp::min(rmin, self.m[ri-1]+self.v[ri-1]);
            }
            li /= 2;
            ri /= 2;
            lmin += self.v[li];
            rmin += self.v[ri];
        }
        let mut min = std::cmp::min(lmin, rmin);
        while li > 1 {
            li /= 2;
            min += self.v[li];
        }
        return min;
    }
}

#[cfg(test)]
mod tests {
    // Note this useful idiom: importing names from outer (for mod tests) scope.
    use super::*;

    #[test]
    fn test_16_f() {
        let mut tree = RangeTree::new(1<<4);
        // 0..8

        for i in 0..16 {
            tree.add(i,i, 1);
        }
        tree.debug();
        
        for i in 0..16 {
            tree.add(0,i, -1)
        }
        tree.debug();


        assert_eq!(tree.get(0,15), -15);
    }

    #[test]
    fn test_8() {
        let mut tree = RangeTree::new(1<<3);
        // 0..32
        tree.add(1,4, 5);
        tree.add(3,6, -6);

        assert_eq!(tree.get(0,2), 0);
        assert_eq!(tree.get(1,2), 5);
        assert_eq!(tree.get(2,4), -1);
        assert_eq!(tree.get(1,7), -6);
    }
    #[test]
    fn test_16() {
        let mut tree = RangeTree::new(1<<4);
        // 0..16
        tree.add(1,9, 5);

        tree.debug();

        tree.add(6,13, -6);

        tree.debug();
        assert_eq!(tree.get(0,5), 0);
        assert_eq!(tree.get(2,5), 5);
        assert_eq!(tree.get(2,9), -1);
        assert_eq!(tree.get(2,15), -6);
    }

    
    #[test]
    fn test_1024() {
        let mut tree = RangeTree::new(1<<10);
        // 0..1024

        for i in 1..999 {
            tree.add(i,i, 1);
        }
        
        for i in 2..1024 {
            tree.add(0,i, -1)
        }

        assert_eq!(tree.get(0,1023), -1022);
        assert_eq!(tree.get(5,5), -1018);
    }
}

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let n: usize = iter.next().unwrap().parse().unwrap();
    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut s: Vec<char> = input.chars().collect();
    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    iter = input.split_whitespace();
    let q: usize = iter.next().unwrap().parse().unwrap();
    let mut tree = RangeTree::new(1<<19);
    let mut sum = 0;
    let mut i = 0;
    for c in &s {
        if *c == 'A' {
            sum += 1;
        } else if *c == 'B' {
            sum -= 1;
        } else {break;}
        i += 1;
        tree.add(i, i, sum);
    }
    for _i in 0..q {
        input = String::new();
        std::io::stdin().read_line(&mut input).unwrap();
        iter = input.split_whitespace();
        let t: usize = iter.next().unwrap().parse().unwrap();
        if t == 1 {
            let i : usize = iter.next().unwrap().parse().unwrap();
            let c : char = iter.next().unwrap().parse().unwrap();
            if s[i-1] == 'A' && c == 'B' {
                tree.add(i, n, -2);
            }
            else if s[i-1] == 'B' && c == 'A' {
                tree.add(i, n, 2);
            }
            s[i-1] = c;
        } else {
            let l : usize = iter.next().unwrap().parse().unwrap();
            let r : usize = iter.next().unwrap().parse().unwrap();
            if tree.get(l, r) >= tree.get(l-1, l-1) {
                println!("Yes");
            } else {
                println!("No");
            }
        }
    }
}