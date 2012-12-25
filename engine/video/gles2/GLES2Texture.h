#ifndef	_GLES2_TEXTURE_H_
#define _GLES2_TEXTURE_H_

namespace easy2d {

	class GLES2Texture : public ITexture
	{
	public:
		GLES2Texture();
		virtual ~GLES2Texture();
		virtual bool create(const char* filename);
		virtual int width(const char* name);
		virtual int height(const char* name);
		virtual void bind(int index);
		virtual unsigned int getTextureId();

	protected:
		unsigned char* loadTGA(const char* filename, int& colorDepth);

	protected:
		GLuint mTexId;
		int mWidth;
		int mHeight;
	};




}	// namespace easy2d


#endif