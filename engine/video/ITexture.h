#ifndef	_ITEXTURE_H_
#define _ITEXTURE_H_

namespace easy2d {

class ITexture
{
public:
	enum EPixelFormat
	{
		PF_RGB,
	};

public:
	virtual ~ITexture(){}

	virtual bool create(const char* filename) = 0;
	virtual int width(const char* name) = 0;
	virtual int height(const char* name) = 0;
	virtual void bind(int index) = 0;
	virtual unsigned int getTextureId() = 0;
};



}	// namespace easy2d


#endif