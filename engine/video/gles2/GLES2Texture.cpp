#include "../ITexture.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"

#include "GLES2Texture.h"

#include <stdio.h>
#include <stdlib.h>


///
//  Macros
//
#define INVERTED_BIT            (1 << 5)

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
		unsigned char* buffer = loadTGA(filename);
		if (buffer == NULL)
			return false;

		glGenTextures ( 1, &mTexId );
		glBindTexture ( GL_TEXTURE_2D, mTexId );

		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		free ( buffer );

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

	unsigned char* GLES2Texture::loadTGA(const char *fileName)
	{
		unsigned char* buffer;
		FILE        *fp;
		TGA_HEADER   Header;

		if ( fopen_s ( &fp, fileName, "rb" ) != 0 )
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

		if ( Header.ColorDepth == 24 )
		{
			RGBTRIPLE *Buffer24;

			Buffer24= (RGBTRIPLE*)malloc(sizeof(RGBTRIPLE) * mWidth * mHeight);

			if(Buffer24)
			{
				int i=0;
				int x,
					y;

				fread(Buffer24, sizeof(RGBTRIPLE), mWidth * mHeight, fp);

				buffer= (unsigned char*) malloc(3 * mWidth * mHeight);

				for ( y = 0; y < mHeight; y++ )
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

					fclose(fp);
					free(Buffer24);
					return buffer;
			}		
		}

		return NULL;
	}


}