use crate::file_saver::save_ppm;
use crate::types::{BackgroundColor, Color24, Params};
use crate::utils::unpack_params;

pub fn create_interpolation(params: Params) {
    let (width, heigh, channels) = unpack_params(params);

    let mut image: Vec<u8> = vec![0; (width * heigh * channels as u32) as usize];
    let mut image_idx = 0;
    let background: BackgroundColor = BackgroundColor::new(vec![
        Color24 {
            red: 0,
            green: 255,
            blue: 0,
        },
        Color24 {
            red: 0,
            green: 0,
            blue: 255,
        },
        Color24 {
            red: 255,
            green: 0,
            blue: 0,
        },
        Color24 {
            red: 255,
            green: 255,
            blue: 0,
        },
    ]);

    for line in 0..heigh {
        for column in 0..width {
            let u = ((100 * column) / width) as f64 / 100.0;
            let v = ((100 * line) / heigh) as f64 / 100.0;

            let point: Color24 = background.sample_uv(u, v);

            image[image_idx] = point.red;
            image[image_idx + 1] = point.green;
            image[image_idx + 2] = point.blue;

            image_idx += 3;
        }
    }

    save_ppm(
        "interpolation.ppm".to_string(),
        image,
        width,
        heigh,
        channels,
    );
}
