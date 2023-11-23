#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
using namespace std;


int main(void)
{
	// Read in pixel data from disk
	vector<unsigned char> pixel_data;

	unsigned char idlength = 0;
	unsigned char colourmaptype = 0;
	unsigned char datatypecode = 0;
	unsigned short int colourmaporigin = 0;
	unsigned short int colourmaplength = 0;
	unsigned char colourmapdepth = 0;
	unsigned short int x_origin = 0;
	unsigned short int y_origin = 0;

	unsigned short int px = 0;
	unsigned short int py = 0;
	unsigned char bitsperpixel = 0;
	unsigned char imagedescriptor = 0;
	vector<char> idstring;

	ifstream in("cat.tga", ios::binary);

	if (!in.is_open())
	{
		cout << "Failed to open TGA file: cat.tga" << endl;
		return false;
	}

	in.read(reinterpret_cast<char*>(&idlength), 1);
	in.read(reinterpret_cast<char*>(&colourmaptype), 1);
	in.read(reinterpret_cast<char*>(&datatypecode), 1);
	in.read(reinterpret_cast<char*>(&colourmaporigin), 2);
	in.read(reinterpret_cast<char*>(&colourmaplength), 2);
	in.read(reinterpret_cast<char*>(&colourmapdepth), 1);
	in.read(reinterpret_cast<char*>(&x_origin), 2);
	in.read(reinterpret_cast<char*>(&y_origin), 2);
	in.read(reinterpret_cast<char*>(&px), 2);
	in.read(reinterpret_cast<char*>(&py), 2);
	in.read(reinterpret_cast<char*>(&bitsperpixel), 1);
	in.read(reinterpret_cast<char*>(&imagedescriptor), 1);

	if (0 != idlength)
	{
		idstring.resize(idlength + 1, '\0');
		in.read(&idstring[0], idlength);
	}

	if (2 != datatypecode)
	{
		cout << "TGA file must be in uncompressed/non-RLE 32-bit RGBA format." << endl;
		return false;
	}
	else
	{
		if (32 != bitsperpixel)
		{
			cout << "TGA file must be in uncompressed/non-RLE 32-bit RGBA format." << endl;
			return false;
		}

		size_t num_bytes = px * py * 4;
		pixel_data.resize(num_bytes);
		in.read(reinterpret_cast<char*>(&pixel_data[0]), num_bytes);
	}


	// March along image
	const float template_width = 2.0f;
	const float template_height = 2.0f;

	const float x_step_size = template_width / static_cast<float>(px - 1);
	const float y_step_size = template_height / static_cast<float>(py - 1);

	const float grid_x_min = -template_width / 2.0f;
	const float grid_y_min = -template_height / 2.0f;

	float grid_x_pos = grid_x_min;

	vector<unsigned char> temp_data(px * py * 4, 0);

	const float curr_x_min = grid_x_min;
	const float curr_y_min = grid_y_min;
	const float curr_x_max = grid_x_min + template_width;
	const float curr_y_max = grid_y_min + template_height;

	const float x_extent = curr_x_max - curr_x_min;
	const float y_extent = curr_y_max - curr_y_min;

	for (short unsigned int x = 0; x < px; x++, grid_x_pos += x_step_size)
	{
		float grid_y_pos = grid_y_min;

		for (short unsigned int y = 0; y < py; y++, grid_y_pos += y_step_size)
		{
			const size_t index = 4 * (y * size_t(px) + x);

			const complex<float> c(1.95f, 0.4f);

			complex<float> pos(grid_x_pos, grid_y_pos);

			pos = pos * c; // Scale and rotate the image

			const float x_location = pos.real() - curr_x_min;
			size_t x_index = static_cast<size_t>(static_cast<float>(px - 1) * (x_location / x_extent));

			if (x_index >= static_cast<size_t>(px))
				x_index = static_cast<size_t>(px - 1);

			const float y_location = pos.imag() - curr_y_min;
			size_t y_index = static_cast<size_t>(static_cast<float>(py - 1) * (y_location / y_extent));

			if (y_index >= static_cast<size_t>(py))
				y_index = static_cast<size_t>(py - 1);

			const size_t new_index = 4 * (y_index * px + x_index);

			temp_data[index + 0] = pixel_data[new_index + 0];
			temp_data[index + 1] = pixel_data[new_index + 1];
			temp_data[index + 2] = pixel_data[new_index + 2];
			temp_data[index + 3] = 255;
		}
	}

	pixel_data = temp_data;
	temp_data.clear();


	// Write modified image to disk
	ofstream out("cat_out.tga", ios::binary);

	if (!out.is_open())
		return -1;

	out.write(reinterpret_cast<char*>(&idlength), 1);
	out.write(reinterpret_cast<char*>(&colourmaptype), 1);
	out.write(reinterpret_cast<char*>(&datatypecode), 1);
	out.write(reinterpret_cast<char*>(&colourmaporigin), 2);
	out.write(reinterpret_cast<char*>(&colourmaplength), 2);
	out.write(reinterpret_cast<char*>(&colourmapdepth), 1);
	out.write(reinterpret_cast<char*>(&x_origin), 2);
	out.write(reinterpret_cast<char*>(&y_origin), 2);
	out.write(reinterpret_cast<char*>(&px), 2);
	out.write(reinterpret_cast<char*>(&py), 2);
	out.write(reinterpret_cast<char*>(&bitsperpixel), 1);
	out.write(reinterpret_cast<char*>(&imagedescriptor), 1);

	out.write(reinterpret_cast<char*>(&pixel_data[0]), px * py * 4 * sizeof(unsigned char));

	out.close();

	return 0;
}