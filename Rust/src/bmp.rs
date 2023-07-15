use std::fs::File;
use std::io::Write;
use rand::Rng;

struct RendererClass {
	width: u32,
	height: u32,
	pix_map: Vec<Vec<(u8,u8,u8,u8)>>,
	pen_color: (u8,u8,u8,u8),
	pen_opacity: f32
}
impl RendererClass {
	fn new(width: u32, height: u32) -> Self {
		let mut columns: Vec<Vec<(u8,u8,u8,u8)>> = Vec::with_capacity(height as usize);
		for _ in 0..width {
			let mut row: Vec<(u8,u8,u8,u8)> = Vec::with_capacity(width as usize);
			for _ in 0..height {
				row.push((255,255,255,255));
			}
		columns.push(row)
		}
		RendererClass {
			width,
			height,
			pix_map: columns,
			pen_color: (0,0,0,255),
			pen_opacity: 1.0
		}
	}

	fn set_color(&mut self, color:(u8,u8,u8,u8)) {
		self.pen_color = color;
	}
	fn set_opacity(&mut self, opacity: f32) {
		self.pen_opacity = opacity
	}

	fn draw_bg(&mut self, color: (u8,u8,u8,u8)) {
		for _ in 0.. self.width {
			let mut row: Vec<(u8,u8,u8,u8)> = Vec::with_capacity(self.width as usize);
			for _ in 0..self.height {
				row.push(color);
			}
			self.pix_map.push(row)
		}
	}

	fn edit_pixel(&mut self, pos_x: u32, pos_y: u32){
		if pos_x < self.width && pos_y < self.height {
			let mut pixel = self.pix_map[pos_y as usize][pos_x as usize];
			pixel = (
				(self.pen_color.3 as f32/255.0 * self.pen_opacity * self.pen_color.0 as f32 + pixel.0 as f32 * (1.0 - self.pen_color.3 as f32 * self.pen_opacity/255.0)) as u8,
				(self.pen_color.3 as f32/255.0 * self.pen_opacity * self.pen_color.1 as f32 + pixel.1 as f32 * (1.0 - self.pen_color.3 as f32 * self.pen_opacity/255.0)) as u8,
				(self.pen_color.3 as f32/255.0 * self.pen_opacity * self.pen_color.2 as f32 + pixel.2 as f32 * (1.0 - self.pen_color.3 as f32 * self.pen_opacity/255.0)) as u8,
				self.pen_color.3
			);
			self.pix_map[pos_y as usize][pos_x as usize] = pixel;
		}
	}
	fn edit_pixel_col(&mut self, pos_x: u32, pos_y: u32, color: (u8,u8,u8,u8)){
		if pos_x < self.width && pos_y < self.height {
			let mut pixel = self.pix_map[pos_y as usize][pos_x as usize];
			pixel = (
				(color.3 as f32/255.0 * self.pen_opacity * color.0 as f32 + pixel.0 as f32 * (1.0 - color.3 as f32 * self.pen_opacity/255.0)) as u8,
				(color.3 as f32/255.0 * self.pen_opacity * color.1 as f32 + pixel.1 as f32 * (1.0 - color.3 as f32 * self.pen_opacity/255.0)) as u8,
				(color.3 as f32/255.0 * self.pen_opacity * color.2 as f32 + pixel.2 as f32 * (1.0 - color.3 as f32 * self.pen_opacity/255.0)) as u8,
				color.3
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

	fn draw_circle(&mut self, point: (u32,u32), radius: f32) {
		for deg in 0..359 {
			self.draw_line_rotlen((point.0 as i32,point.1 as i32), deg as f32, radius);
			self.draw_line_rotlen((point.0 as i32,point.1 as i32), deg as f32 + 0.5, radius);
		}
	}

	fn draw_mono_noise(&mut self, per: f32, color_low: (u8,u8,u8,u8), color_high: (u8,u8,u8,u8)) {
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
				self.edit_pixel_col(x, y, (rand::thread_rng().gen::<u8>(),rand::thread_rng().gen::<u8>(),rand::thread_rng().gen::<u8>(),255));
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
				file.write_all(&[pixel.2, pixel.1, pixel.0, pixel.3])?;	// Blue, Green, Red, Alpha
			}
		}
		Ok(())
	}
}

fn main() {
	let mut renderer = RendererClass::new(256,256);
	renderer.set_color((255,0,0,255));
	renderer.draw_circle((127,127), 40.0);
	let _ = (&renderer).store_bmp("Output.bmp");
}