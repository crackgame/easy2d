#include "../ITexture.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "FreeImage/FreeImage.h"

#include "GLES2Texture.h"

#include <stdio.h>
#include <stdlib.h>


///
//  Macros
//
#define INVERTED_BIT            (1 << 5)
#define INVERTED_RGB            (1 << 3)

///
//  Types
//
#pragma pack(push,x1)                            // Byte alignment (8-bit)
#pragma pack(1)

typedef struct
{
	unsigned char  IdSize,
		MapType,
		ImageType;
	unsigned short PaletteStart,
		PaletteSize;
	unsigned char  PaletteEntryDepth;
	unsigned short X,
		Y,
		Width,
		Height;
	unsigned char  ColorDepth,
		Descriptor;

} TGA_HEADER;

#pragma pack(pop,x1)



namespace easy2d {

	GLES2Texture::GLES2Texture()
	{
		mTexId = 0;
		mWidth = 0;
		mHeight = 0;
	}

	GLES2Texture::~GLES2Texture()
	{
		glDeleteTextures (1, &mTexId);
	}

	bool GLES2Texture::create(const char* filename)
	{
		int colorDepth = 0;
		unsigned char* buffer = loadImage(filename, colorDepth);
		if (buffer == NULL)
			return false;

		glGenTextures ( 1, &mTexId );
		glBindTexture ( GL_TEXTURE_2D, mTexId );

		if (colorDepth == 32) {
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
		} else {
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer );
		}
		
		//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		delete[] buffer;

		return true;
	}

	void GLES2Texture::bind(int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, mTexId);
	}

	unsigned int GLES2Texture::getTextureId()
	{
		return mTexId;
	}

	int GLES2Texture::width(const char* name)
	{
		return mWidth;
	}

	int GLES2Texture::height(const char* name)
	{
		return mHeight;
	}

	unsigned char* GLES2Texture::loadTGA(const char* filename, int& colorDepth)
	{
		unsigned char* buffer;
		FILE        *fp;
		TGA_HEADER   Header;

		if ( fopen_s ( &fp, filename, "rb" ) != 0 )
		{
			return NULL;
		}

		if ( fp == NULL )
		{
			return NULL;
		}

		fread ( &Header, sizeof(TGA_HEADER), 1, fp );

		mWidth = Header.Width;
		mHeight = Header.Height;
		colorDepth = Header.ColorDepth;

		if ( Header.ColorDepth == 24 ) {
			// 24位图片读取
			RGBTRIPLE *Buffer24;

			Buffer24 = new RGBTRIPLE[mWidth * mHeight];

			if(Buffer24)
			{
				int i=0;
				int x,
					y;

				fread(Buffer24, sizeof(RGBTRIPLE), mWidth * mHeight, fp);

				buffer= new unsigned char[3 * mWidth * mHeight];

				for ( y = 0; y < mHeight; y++ ) {
					for( x = 0; x < mWidth; x++ )
					{
						int Index= y * mWidth + x;

						if(!(Header.Descriptor & INVERTED_BIT))
							Index= (mHeight - 1 - y) * mWidth + x;

						buffer[(i * 3)]=      Buffer24[Index].rgbtRed;
						buffer[(i * 3) + 1]=  Buffer24[Index].rgbtGreen;
						buffer[(i * 3) + 2]=  Buffer24[Index].rgbtBlue;

						i++;
					}
				}

				fclose(fp);
				delete[] Buffer24;
				return buffer;
			}		
		} else if ( Header.ColorDepth == 32 ) {
			// 32位图片读取
			RGBQUAD *Buffer32;

			Buffer32= new RGBQUAD[mWidth * mHeight];

			if (Buffer32) {
				int i=0;
				int x, y;

				fread(Buffer32, sizeof(RGBQUAD), mWidth * mHeight, fp);

				buffer= new unsigned char[4 * mWidth * mHeight];

				for(y = 0; y < mHeight; y++) {
					for(x = 0; x < mWidth; x++)
					{
						int Index= y * mWidth + x;

						if(!(Header.Descriptor & INVERTED_BIT))
							Index= (mHeight - 1 - y) * mWidth + x;

#if 1
						buffer[(i * 4)]=      Buffer32[Index].rgbReserved;
						buffer[(i * 4) + 1]=  Buffer32[Index].rgbRed;
						buffer[(i * 4) + 2]=  Buffer32[Index].rgbGreen;
						buffer[(i * 4) + 3]=  Buffer32[Index].rgbBlue;
#else
						buffer[(i * 4)]=      Buffer32[Index].rgbRed;
						buffer[(i * 4) + 1]=  Buffer32[Index].rgbGreen;
						buffer[(i * 4) + 2]=  Buffer32[Index].rgbBlue;
						buffer[(i * 4) + 3]=  Buffer32[Index].rgbReserved;
#endif

						i++;
					}
				}

				fclose(fp);
				delete[] Buffer32;
				return buffer;
			}	
		}

		return NULL;
	}

	unsigned char* GLES2Texture::loadImage(const char *filename, int& colorDepth)
	{
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);
		//pointer to the image data
		BYTE* bits(0);
		//image width and height
		unsigned int width(0), height(0);

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(filename, 0);
		//if still unknown, try to guess the file format from the file extension
		if(fif == FIF_UNKNOWN) 
			fif = FreeImage_GetFIFFromFilename(filename);
		//if still unkown, return failure
		if(fif == FIF_UNKNOWN)
			return NULL;

		//check that the plugin has reading capabilities and load the file
		if(FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		//if the image failed to load, return failure
		if(!dib)
			return NULL;

		int datasize = FreeImage_GetDIBSize(dib);
		//retrieve the image data
		bits = FreeImage_GetBits(dib);
		//get the image width and height
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		//get the image bpp
		colorDepth = FreeImage_GetBPP(dib);
		//if this somehow one of these failed (they shouldn't), return failure
		if((bits == 0) || (width == 0) || (height == 0))
			return NULL;

		mWidth = width;
		mHeight = height;

		unsigned char* buffer = new unsigned char[datasize];
		memcpy(buffer, bits, datasize);

		//Free FreeImage's copy of the data
		FreeImage_Unload(dib);

		return buffer;
	}


}