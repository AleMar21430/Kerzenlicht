#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // hide console window on Windows in release

use eframe::egui;

struct MyApp {
	name: String,
	age: u32,
}

impl Default for MyApp {
	fn default() -> Self {
		Self {
			name: "Arthur".to_owned(),
			age: 42,
		}
	}
}

impl eframe::App for MyApp {
	fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
		egui::CentralPanel::default().show(ctx, |ui| {
			ui.heading("Rusterizer");
			ui.horizontal(|ui| {
				let name_label = ui.label("Your name: ");
				ui.text_edit_singleline(&mut self.name)
					.labelled_by(name_label.id);
			});
			ui.add(egui::Slider::new(&mut self.age, 0..=120).text("age"));
			if ui.button("Click each year").clicked() {
				self.age += 1;
			}
			ui.label(format!("Hello '{}', age {}", self.name, self.age));
		});
	}
}

fn main() -> Result<(), eframe::Error> {
	let options = eframe::NativeOptions {
		initial_window_size: Some(egui::vec2(320.0, 240.0)),
		..Default::default()
	};
	eframe::run_native(
		"Rusterizer",
		options,
		Box::new(|_cc| Box::<MyApp>::default()),
	)
}