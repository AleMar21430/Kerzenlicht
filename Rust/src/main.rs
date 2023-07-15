use std::fs::File;
use std::io::Write;
use half::f16;
use rand::Rng;
use openexr::prelude::*;

struct RendererClass {
	width: u32,
	height: u32,
	pix_map: Vec<Vec<Rgba>>,
	pen_color: Rgba,
	pen_opacity: f32
}
impl RendererClass {
	fn new(width: u32, height: u32) -> Self {
		let mut columns: Vec<Vec<Rgba>> = Vec::with_capacity(height as usize);
		let color = Rgba::from_f32(0.2, 0.2, 0.2, 1.0);
		for _ in 0..width {
			let mut row: Vec<Rgba> = Vec::with_capacity(width as usize);
			for _ in 0..height {
				row.push(color);
			}
		columns.push(row)
		}
		RendererClass {
			width,
			height,
			pix_map: columns,
			pen_color: Rgba::from_f32(0.0, 0.0, 0.0, 1.0),
			pen_opacity: 1.0
		}
	}

	fn set_color(&mut self, color:Rgba) {
		self.pen_color = color;
	}
	fn set_opacity(&mut self, opacity: f32) {
		self.pen_opacity = opacity
	}

	fn draw_bg(&mut self, color: Rgba) {
		for _ in 0.. self.width {
			let mut row: Vec<Rgba> = Vec::with_capacity(self.width as usize);
			for _ in 0..self.height {
				row.push(color);
			}
			self.pix_map.push(row)
		}
	}

	fn edit_pixel(&mut self, pos_x: u32, pos_y: u32){
		if pos_x < self.width && pos_y < self.height {
			let mut pixel = self.pix_map[pos_y as usize][pos_x as usize];
			let opacity = f16::from_f32(self.pen_opacity);
			pixel = Rgba::from_f32(
				(self.pen_color.a * opacity * self.pen_color.r + pixel.r * (f16::from_f32(1.0) - self.pen_color.a * opacity)).to_f32(),
				(self.pen_color.a * opacity * self.pen_color.g + pixel.g * (f16::from_f32(1.0) - self.pen_color.a * opacity)).to_f32(),
				(self.pen_color.a * opacity * self.pen_color.b + pixel.b * (f16::from_f32(1.0) - self.pen_color.a * opacity)).to_f32(),
				self.pen_color.a.to_f32()
			);
			self.pix_map[pos_y as usize][pos_x as usize] = pixel;
		}
	}
	fn edit_pixel_col(&mut self, pos_x: u32, pos_y: u32, color: Rgba){
		if pos_x < self.width && pos_y < self.height {
			let mut pixel = self.pix_map[pos_y as usize][pos_x as usize];
			let opacity = f16::from_f32(self.pen_opacity);
			pixel = Rgba::from_f32(
				(color.a * opacity * color.r + pixel.r * (f16::from_f32(1.0) - color.a * opacity)).to_f32(),
				(color.a * opacity * color.g + pixel.g * (f16::from_f32(1.0) - color.a * opacity)).to_f32(),
				(color.a * opacity * color.b + pixel.b * (f16::from_f32(1.0) - color.a * opacity)).to_f32(),
				color.a.to_f32()
			);
			self.pix_map[pos_y as usize][pos_x as usize] = pixel;
		}
	}

	fn draw_point(&mut self, point: (u32,u32)) {
		self.edit_pixel(point.0, point.1)
	}

	fn draw_line_pos(&mut self, start: (i32,i32), end: (i32,i32)) {
		let (x1, y1) = start;
		let (x2, y2) = end;

		let dx: isize = (x2 as isize - x1 as isize).abs();
		let dy: isize = (y2 as isize - y1 as isize).abs();
		let mut err: isize = dx - dy;

		let mut x: i32 = x1 ;
		let mut y: i32 = y1;

		while x != x2 || y != y2 {
			self.draw_point((x as u32, y as u32));

			let e2: isize = 2 * err;
			if e2 > -dy {
				err -= dy;
				x = x.wrapping_add(if x1 < x2 { 1 } else { -1 });
			}
			if e2 < dx {
				err += dx;
				y = y.wrapping_add(if y1 < y2 { 1 } else { -1 });
			}
		}
		self.draw_point((x2 as u32,y2 as u32));
	}
	fn draw_line_rotlen(&mut self, start: (i32,i32), rot: f32, len: f32) {
		self.draw_line_pos(start, ((start.0 as f32 + len * rot.to_radians().cos()) as i32, (start.1 as f32 + len * rot.to_radians().sin()) as i32));
	}

	fn draw_rect_xyxy(&mut self, point: (u32,u32), size: (u32,u32)) {
		for x in 0..size.0 {
			for y in 0..size.1 {
				self.edit_pixel(point.0+x, point.1+y)
			}
		}
	}
	fn draw_rect_bound(&mut self, bottom_left: (u32,u32), top_right: (u32,u32)) {
		for x in 0..(top_right.0 - bottom_left.0) {
			for y in 0..(top_right.1 - bottom_left.1)  {
				self.edit_pixel(bottom_left.0+x, top_right.1+y)
			}
		}
	}

	fn draw_circle(&mut self, point: (i32,i32), radius: f32) {
		let mut x: f32 = radius;
		let mut y:i32 = 0;
		let mut err: i32 = 0;
	
		while x  as i32>= y  {
			// Set the pixels for each octant of the circle
			self.edit_pixel((point.0 + x as i32) as u32, (point.1 + y) as u32);
			self.edit_pixel((point.0 + y) as u32, (point.1 + x as i32) as u32);
			self.edit_pixel((point.0 - y) as u32, (point.1 + x as i32) as u32);
			self.edit_pixel((point.0 - x as i32) as u32, (point.1 + y) as u32);
			self.edit_pixel((point.0 - x as i32) as u32, (point.1 - y) as u32);
			self.edit_pixel((point.0 - y) as u32, (point.1 - x as i32) as u32);
			self.edit_pixel((point.0 + y) as u32, (point.1 - x as i32) as u32);
			self.edit_pixel((point.0 + x as i32) as u32, (point.1 - y) as u32);
	
			// Update coordinates
			y += 1;
			err += 1 + 2 * y;
			if 2 * (err - x as i32) + 1 > 0 {
				x -= 1.0;
				err += 1 - 2 * x as i32;
			}
		}
	}

	fn draw_mono_noise(&mut self, per: f32, color_low: Rgba, color_high: Rgba) {
		for x in 0.. self.width {
			for y in 0..self.height {
				if rand::thread_rng().gen::<f32>() < per {
					self.edit_pixel_col(x, y, color_high);
				} else {
					self.edit_pixel_col(x, y, color_low);
				}
			}
		}
	}
	fn draw_white_noise(&mut self) {
		for x in 0.. self.width {
			for y in 0..self.height {
				self.edit_pixel_col(x, y, (Rgba::from_f32(rand::thread_rng().gen::<f32>(),rand::thread_rng().gen::<f32>(),rand::thread_rng().gen::<f32>(),1.0)));
			}
		}
	}

	fn store_bmp(&self, filename: &str) -> std::io::Result<()> {
		let mut file: File = File::create(filename)?;
		let filesize: u32 = self.width * self.height * 4;
		// BMP header (14 bytes)
		file.write_all(b"BM")?;							// Signature
		file.write_all(&(filesize+ 54).to_le_bytes())?;	// File size
		file.write_all(&[0, 0, 0, 0])?;					// Reserved
		file.write_all(&54u32.to_le_bytes())?;				// Pixel data offset
		// DIB header (40 bytes)
		file.write_all(&40u32.to_le_bytes())?;				// DIB header size
		file.write_all(&self.width.to_le_bytes())?;		// Image width
		file.write_all(&self.height.to_le_bytes())?;		// Image height
		file.write_all(&1u16.to_le_bytes())?;				// Color planes
		file.write_all(&32u16.to_le_bytes())?;				// Bits per pixel
		file.write_all(&0u32.to_le_bytes())?;				// Compression method
		file.write_all(&filesize.to_le_bytes())?;			// Image size
		file.write_all(&2835u32.to_le_bytes())?;			// Horizontal resolution (pixels per meter)
		file.write_all(&2835u32.to_le_bytes())?;			// Vertical resolution (pixels per meter)
		file.write_all(&0u32.to_le_bytes())?;				// Colors in color palette
		file.write_all(&0u32.to_le_bytes())?;				// Important colors
		// Pixel data
		for row in &self.pix_map {
			for pixel in row {
				file.write_all(&[(pixel.b.to_f32()*255.0) as u8, (pixel.g.to_f32()*255.0) as u8, (pixel.r.to_f32()*255.0) as u8, (pixel.a.to_f32()*255.0) as u8])?;	// Blue, Green, Red, Alpha
			}
		}
		Ok(())
	}
	fn store_exr(&mut self, filename: &str) -> Result<(), Box<dyn std::error::Error>> {
		let header = Header::from_dimensions(self.width as i32, self.height as i32);
		let mut file = RgbaOutputFile::new(filename, &header, RgbaChannels::WriteRgba, 1,)?;
		let flattened: Vec<Rgba> = self.pix_map.clone().into_iter().flatten().collect();
		let pixels: &[Rgba] = flattened.as_slice();
		file.set_frame_buffer(pixels, 1, self.width as usize)?;
		unsafe {
			file.write_pixels(self.height as i32)?;
		}
		Ok(())
	}
}

fn main() {
	let mut renderer = RendererClass::new(256,256);
	renderer.set_color(Rgba::from_f32(1.0,0.0,0.0,1.0));
	renderer.draw_circle((127,127), 40.0);
	let bmp = renderer.store_bmp("Output.bmp");
	let exr = renderer.store_exr("Output.exr");
}