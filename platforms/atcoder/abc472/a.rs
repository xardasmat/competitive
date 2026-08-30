use std::io;

fn main() -> io::Result<()> {
    let mut buffer = String::new();
    io::stdin().read_line(&mut buffer)?;
    let modified_buffer: String = buffer.chars().map(|c| {match c {
            'A' => 'A',
            '\n' => '\n',
            '\r' => '\r',
            _ => '.'
        }}).collect();
    println!("{}", modified_buffer);
    Ok(())
}