mod file_saver;
mod first_task;
mod types;
mod second_task;
mod utils;

use crate::first_task::create_gradient;
use crate::types::Params;
use std::io::stdin;
use crate::second_task::create_interpolation;

fn main() {
    let (choosed, parsed_params) = input_parameters();

    match choosed {
        1 => create_gradient(parsed_params),
        2 => create_interpolation(parsed_params),
        _ => panic!("Can't do this")
    }
}

fn input_parameters() -> (i32, Params) {
    println!(
        "Choose the task you want to run:\n\
        1 - Create a gradient image\n\
        2 - Create a interpolation image"
    );
    let mut input_line: String = String::new();
    stdin()
        .read_line(&mut input_line)
        .expect("Failed to read line");
    let choosed: i32 = input_line
        .trim()
        .parse::<i32>()
        .expect("Input not an integer");

    println!(
        "Set the width and heigh of the image you want to generate\n\
        Ex: 400 500"
    );

    let mut input_params: String = String::new();
    stdin()
        .read_line(&mut input_params)
        .expect("Failed to read line");
    let params: Vec<&str> = input_params.trim().split(' ').collect();
    let mut parsed_params = Params {
        width: params[0].parse::<u32>().expect("Couldn't parse width"),
        height: params[1].parse::<u32>().expect("Couldn't parse height"),
        channels: 3,
    };

    println!(
        "You want to generate a PPM or PNG?\n\
        1 - PPM\n\
        2 - PNG"
    );

    let mut input_file_type: String = String::new();
    stdin()
        .read_line(&mut input_file_type)
        .expect("Failed to read file type requested");
    let filetype: u8 = input_file_type
        .trim()
        .parse::<u8>()
        .expect("Couldn't parse filetype must be a number");

    match filetype {
        1 => parsed_params.channels = 3,
        // 2 => parsed_params.channels = 4,
        _ => panic!("Couldn't parse filetype (must be 1)"),
    }
    (choosed, parsed_params)
}
