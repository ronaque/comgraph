use std::fs::File;
use std::io::Write;
use std::path::Path;

pub fn save_ppm(filename: String, buffer: Vec<u8>, width: u32, heigh: u32, channels: u8) {
    let path = Path::new(filename.as_str());

    let mut file = match File::create(&path) {
        Err(why) => panic!("Couldn't create {}: {}", path.display(), why),
        Ok(file) => file,
    };

    file.write(b"P3\n").expect("Couldn't write to file");
    file.write(format!("{width} {heigh}\n").as_bytes())
        .expect("Couldn't write to file");
    file.write(b"255\n").expect("Couldn't write to file");
    for idx in (0..buffer.len()).step_by(3) {
        file.write(
            format!("{} {} {} \n", buffer[idx], buffer[idx + 1], buffer[idx + 2]).as_bytes(),
        )
        .expect("Couldn't write to file");
    }
}