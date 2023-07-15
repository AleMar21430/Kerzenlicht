use std::fs::File;
use std::io::Write;

fn write_bmp(filename: &str, width: u32, height: u32, pixels: &[(u8, u8, u8, u8)]) -> std::io::Result<()> {
	let mut file = File::create(filename)?;

	file.write_all(b"#?RADIANCE\n")?;
	file.write_all(b"FORMAT=32-bit_rle_rgbe\n\n")?;
	file.write_all(format!("-Y {}", height).as_bytes())?;
	file.write_all(format!("+X {}", width).as_bytes())?;

	for pixel in pixels {
		file.write_all(&pixel.0.to_le_bytes())?;
		file.write_all(&pixel.1.to_le_bytes())?;
		file.write_all(&pixel.2.to_le_bytes())?;
		file.write_all(&pixel.3 .to_le_bytes())?;
	}

	Ok(())
}

fn main() {
	let width: u32 = 256;
	let height: u32 = 256;

	let pixels: Vec<(u8, u8, u8, u8)> = (0..width * height).map(|_| (50, 50, 50, 128)).collect();

	let filename: &str = "Output.hdr";
	if let Err(err) = write_bmp(filename, width, height, &pixels) {
		eprintln!("Error creating image: {}", err);
	} else {
		println!("Image '{}' created successfully.", filename);
	}
}
