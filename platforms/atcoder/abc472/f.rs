
fn diff(left : i32, right : i32, a : &Vec<i64>) -> i64{
    if left < right {
        return a[right as usize] - a[left as usize];
    } else {
        return a.last().unwrap() - a[left as usize] + a[right as usize] - a[0];
    }
}

fn main() {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let mut first_line = input.split_whitespace();
    let n: i32 = first_line.next().unwrap().parse().unwrap();
    let q: i32 = first_line.next().unwrap().parse().unwrap();
    
    let mut p : Vec<(i64, i64)> = vec![];
    let mut a : Vec<i64> = vec![0];
    let mut cx : Vec<i64> = vec![0];
    let mut cy : Vec<i64> = vec![0];


    for i in 0..n {
        input = String::new();
        std::io::stdin().read_line(&mut input).unwrap();
        let mut first_line = input.split_whitespace();
        let x: i64 = first_line.next().unwrap().parse().unwrap();
        let y: i64 = first_line.next().unwrap().parse().unwrap();
        p.push((x,y));
        if i > 0 {
            let (x1, y1) = p[(i-1) as usize];
            a.push(x1*y-y1*x + a.last().unwrap());
            cx.push((x+x1)*(x1*y-y1*x) + cx.last().unwrap());
            cy.push((y+y1)*(x1*y-y1*x) + cy.last().unwrap());
        }
    }
    {
        let (x, y) = p[0];
        let (x1, y1) = p.last().unwrap();
        a.push(x1*y-y1*x + a.last().unwrap());
        cx.push((x+x1)*(x1*y-y1*x) + cx.last().unwrap());
        cy.push((y+y1)*(x1*y-y1*x) + cy.last().unwrap());
    }
    for _i in 0..q {
        input = String::new();
        std::io::stdin().read_line(&mut input).unwrap();
        let mut first_line = input.split_whitespace();
        let u: i32 = first_line.next().unwrap().parse().unwrap();
        let v: i32 = first_line.next().unwrap().parse().unwrap();

        let mut aa = diff(u-1, v-1, &a);
        let mut ccx = diff(u-1, v-1, &cx);
        let mut ccy = diff(u-1, v-1, &cy);

        let (x, y) = p[(v-1) as usize];
        let (x1, y1) = p[(u-1) as usize];
        aa += x*y1-y*x1;
        ccx += (x+x1)*(x*y1-y*x1);
        ccy += (y+y1)*(x*y1-y*x1);
        println!("{:.20} {:.20}",
            (ccx as f64)/(aa as f64)/3.0,
            (ccy as f64)/(aa as f64)/3.0);
    }
}