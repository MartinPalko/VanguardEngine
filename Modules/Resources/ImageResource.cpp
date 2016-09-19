#include "ImageResource.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


#include "png.h"

namespace Vanguard
{

	TYPE_DEFINITION(ImageResource, Resource);

	ImageResource::ImageResource()
	{

	}

	ImageResource::~ImageResource()
	{

	}

	bool ReadPNG(const FilePath& aFile)
	{
		// Work in progres
		return false;

		String pathName = aFile.GetFullPathName();
		const char* file_name = pathName.GetCharPointer();

		int width, height;
		png_byte color_type;
		png_byte bit_depth;

		png_structp png_ptr;
		png_infop info_ptr;
		int number_of_passes;
		png_bytep * row_pointers;

		unsigned char header[8];

		// Check the header to make sure the file is a PNG
		FILE *fp = fopen(file_name, "rb");
		if (!fp)
		{
			LOG_ERROR("PNG file could not be opened for reading: " + aFile, "ImageResource");
			return false;
		}

		fread(header, 1, 8, fp);
		if (png_sig_cmp(header, 0, 8))
		{
			LOG_ERROR("File is not a valid PNG file: " + aFile, "ImageResource");
			return false;
		}


		/* initialize stuff */
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		if (!png_ptr)
		{
			LOG_ERROR("File read struct creation failed: " + aFile, "ImageResource");
			return false;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			LOG_ERROR("PNG info struct creation failed: " + aFile, "ImageResource");
			return false;
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			LOG_ERROR("Error during PNG init_io: " + aFile, "ImageResource");
			return false;
		}

		png_init_io(png_ptr, fp);
		png_set_sig_bytes(png_ptr, 8);

		png_read_info(png_ptr, info_ptr);

		width = png_get_image_width(png_ptr, info_ptr);
		height = png_get_image_height(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);
		bit_depth = png_get_bit_depth(png_ptr, info_ptr);

		number_of_passes = png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr, info_ptr);


		// Read file
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			LOG_ERROR("Error during PNG read: " + aFile, "ImageResource");
			return false;
		}

		row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
		for (int y = 0; y < height; y++)
			row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

		png_read_image(png_ptr, row_pointers);

		fclose(fp);
	}

	bool ImageResource::LoadResource()
	{
		FilePath file(GetResourcePath());
		StringID ext = file.GetFileExtension();

		switch (ext.GetHash())
		{
		case CRC32_STR(".png"):
			return ReadPNG(file);
		default:
			break;
		}

		return true;
	}

	bool ImageResource::UnloadResource()
	{
		return false;
	}

	bool ImageResource::IsLoaded()
	{
		return false;
	}

	size_t ImageResource::GetSize()
	{
		return size_t();
	}
}