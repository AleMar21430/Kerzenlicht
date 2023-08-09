#include "Shader.h"

Texture::Texture() {
	Width = 0;
	Height = 0;
	Pixmap = vector(0, vector(0, Rgba()));
}

void Texture::loadfromBitmap(string P_File_Path) {
	ifstream file(P_File_Path, ios::binary);

	char header[54];
	file.read(header, 54);

	Width = *reinterpret_cast<uint16_t*>(&header[18]);
	Height = *reinterpret_cast<uint16_t*>(&header[22]);

	uint32_t rowSize = ((Width * 3 + 3) & (~3)); // Assuming 24 bits per pixel

	Pixmap = vector(Width, vector<Rgba>(Height));

	for (int y = 0; y < Height; y++) {
		vector<uint8_t> rowBuffer(rowSize);
		file.read(reinterpret_cast<char*>(rowBuffer.data()), rowSize);

		for (int x = 0; x < Width; x++) {
			Rgba pixel;
			pixel.B = float(rowBuffer[x * 3]) / 255.0;
			pixel.G = float(rowBuffer[x * 3 + 1]) / 255.0;
			pixel.R = float(rowBuffer[x * 3 + 2]) / 255.0;
			pixel.A = 1.0; // Assuming no alpha channel in BMP, set alpha to 255 (fully opaque)
			Pixmap[x][y] = pixel;
		}
	}
	file.close();
}

Rgba Texture::getColor(Vec2 P_UV) const {
	if (P_UV.X >= 0 && P_UV.X <= 1 && P_UV.Y >= 0 && P_UV.Y <= 1)
		return Pixmap[uint16_t(P_UV.X * double(Width))][uint16_t(P_UV.Y * double(Height))];
	else
		return Rgba(1, 0, 1, 1);
}

Shader::Shader() {
	Albedo = Texture();
}