use crate::file_saver::save_ppm;
use crate::types::Params;
use crate::utils::unpack_params;

pub fn create_gradient(params: Params) {
    let (width, heigh, channels) = unpack_params(params);

    let mut image: Vec<u8> = vec![0; (width * heigh * channels as u32) as usize];
    const MAX_CHANNEL_VALUE: u8 = 255;
    const BLUE_CHANNEL_VALUE: u8 = (0.2 * MAX_CHANNEL_VALUE as f32) as u8;

    let mut image_idx = 0;
    for line in 0..heigh {
        let green_channel_value: u8 = ((255 * line) / width) as u8;
        for column in 0..width {
            let red_channel_value: u8 = ((255 * column) / width) as u8;

            image[image_idx] = red_channel_value;
            image[image_idx + 1] = green_channel_value;
            image[image_idx + 2] = BLUE_CHANNEL_VALUE;
            image_idx += 3;
        }
    }

    save_ppm("gradient.ppm".to_string(), image, width, heigh, channels);
}