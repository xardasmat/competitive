

#[derive(Debug, Copy, Clone)]
struct Linf {
    a:i64,
    b:i64,
}

impl std::ops::Add<Linf> for Linf {
    type Output = Linf;

    fn add(self, rhs: Linf) -> Linf {
        Linf{a: self.a+rhs.a, b: self.b+rhs.b}
    }
}

impl std::ops::Sub<Linf> for Linf {
    type Output = Linf;

    fn sub(self, rhs: Linf) -> Linf {
        Linf{a: self.a-rhs.a, b: self.b-rhs.b}
    }
}

struct Range {
    min : i64,
    max: i64,
}

// (al  (bl   ar)   br) -> (bl ar)
// (al   ar)  (bl    br) -> (bl ar) == Nil
fn con(a : Range, b : Range) -> Range {
    Range {
        min : std::cmp::max(a.min, b.min),
        max : std::cmp::min(a.max, b.max),
    }
}

fn div_ceil(a : i64, b : i64) -> i64 {
    if a%b == 0 || (a>0) != (b>0) { return a/b; }
    else { return a/b+1 };
}

fn div_floor(a : i64, b : i64) -> i64 {
    if a%b == 0 || (a>0) == (b>0) { return a/b; }
    else { return a/b-1 };
}

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let n: usize = first_line.next().unwrap().parse().unwrap();
    
    input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let a : Vec<usize> = input.split_whitespace().map(|word| word.parse::<usize>().unwrap()).collect();
    let mut l = Vec::<Linf>::new();
    for aa in &a {
        l.push(Linf{a: 1, b: -(*aa as i64)});
    }

    let inf = 1000000000000000i64;

    let mut range = Range{ min: 0, max: inf};
    for i in (1..=n).rev() {
        if l[i-1].a == 0 {
            if l[i-1].b < 0 {
                range = Range{ min: 1, max: 0};
            }
        }
        else if l[i-1].a > 0 {
            range = con(range, Range{ min: div_ceil(-l[i-1].b, l[i-1].a), max: inf});
        } else {
            range = con(range, Range{ min: 0, max: div_floor(-l[i-1].b, l[i-1].a) });
        }
        // println!("({}_{}) / T_{} = {}x+{}", range.min, range.max, i, l[i-1].a, l[i-1].b);
        if i == 1 { continue; }
        let mut div = 1;
        while div*div <= i {
            if i%div != 0 { div += 1; continue; }
            l[div-1] = l[div-1] - l[i-1];
            // println!("  ::  T_{} = {}x+{}", div, l[div-1].a, l[div-1].b);
            if div*div != i && div != 1 {
                l[i/div-1] = l[i/div-1] - l[i-1];
                // println!("  ::  T_{} = {}x+{}", i/div, l[i/div-1].a, l[i/div-1].b);
            }
            div += 1;
        }
    }
    // println!("({}_{}) -> {}", range.min, range.max, range.min - (a[0] as i64));
    if range.min <= range.max {
        println!("{}", range.min - (a[0] as i64));
    } else {
        println!("-1");
    }
}