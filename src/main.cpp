#include <stdio.h>
#include <stdlib.h>

#include "../libjpeg/include/jpeglib.h"


int main(int argc, char** argv) {

	if (argc != 3) {
		printf("incorrect parameters, must be:\n");
		printf("jpeg.exe [inputfile] [outputfile]\n");
		return 1;
	}

	unsigned char* image_buffer;

	/* decompress */

	jpeg_decompress_struct decompress_info;
	jpeg_error_mgr jerr;

	decompress_info.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&decompress_info);

	FILE* file_in = fopen(argv[1], "rb");
	jpeg_stdio_src(&decompress_info, file_in);
	jpeg_read_header(&decompress_info, TRUE);

	jpeg_start_decompress(&decompress_info);
	image_buffer = new unsigned char[decompress_info.image_height * decompress_info.image_width  * decompress_info.num_components];
	unsigned char* ptr = image_buffer; 
	int step = decompress_info.image_width * decompress_info.num_components;
	while (decompress_info.output_scanline < decompress_info.image_height) {
		jpeg_read_scanlines(&decompress_info, &ptr, 1);
		ptr += step;
	}
	jpeg_finish_decompress(&decompress_info);

	fclose(file_in);

	/* compress */

	int quality = 93;
	jpeg_compress_struct compress_info;
	compress_info.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&compress_info);

	FILE* file_out = fopen(argv[2], "wb");
	jpeg_stdio_dest(&compress_info, file_out);

	compress_info.image_width = decompress_info.image_width;
	compress_info.image_height = decompress_info.image_height;
	compress_info.input_components = decompress_info.num_components;
	compress_info.in_color_space = decompress_info.out_color_space;
	jpeg_destroy_decompress(&decompress_info);

	jpeg_set_defaults(&compress_info);
	jpeg_set_quality(&compress_info, quality, TRUE);

	jpeg_start_compress(&compress_info, TRUE);
	ptr = image_buffer; 
	step = compress_info.image_width * compress_info.num_components;
	while (compress_info.next_scanline < compress_info.image_height) {
		jpeg_write_scanlines(&compress_info, &ptr, 1);
		ptr += step;
	}
	jpeg_finish_compress(&compress_info);

	jpeg_destroy_compress(&compress_info);
	fclose(file_out);
	delete[] image_buffer;

	printf("done\n");
	return 0;
}
