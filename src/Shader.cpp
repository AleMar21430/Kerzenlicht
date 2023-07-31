#include "Shader.h"

Texture::Texture() {
	Width = 0;
	Height = 0;
	Pixmap = vector(0, vector(0, Rgba()));
}

void Texture::loadfromBitmap(string P_File_Path) {
	ifstream file(P_File_Path, ios::binary);
	if (!file) {
		cerr << "Error: Unable to open the file " << P_File_Path << endl;
	}

	char header[54];
	file.read(header, 54);
	if (header[0] != 'B' || header[1] != 'M') {
		cerr << "Error: Not a valid BMP file" << endl;
	}

	Width = *reinterpret_cast<uint16_t*>(&header[18]);
	Height = *reinterpret_cast<uint16_t*>(&header[22]);

	uint32_t rowSize = ((Width * 3 + 3) & (~3)); // Assuming 24 bits per pixel

	Pixmap = vector(Width, vector<Rgba>(Height));

	for (int y = Height - 1; y >= 0; --y) {
		vector<uint8_t> rowBuffer(rowSize);
		file.read(reinterpret_cast<char*>(rowBuffer.data()), rowSize);

		for (int x = 0; x < Width; ++x) {
			Rgba pixel;
			pixel.B = rowBuffer[x * 3];
			pixel.G = rowBuffer[x * 3 + 1];
			pixel.R = rowBuffer[x * 3 + 2];
			pixel.A = 1.0; // Assuming no alpha channel in BMP, set alpha to 255 (fully opaque)
			Pixmap[y][x] = pixel;
		}
	}
	file.close();

}