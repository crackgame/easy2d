#ifndef	_GLES2_VIDEO_H_
#define _GLES2_VIDEO_H_

namespace easy2d {

	class GLES2Video : public IVideo
	{
	public:
		GLES2Video();
		virtual ~GLES2Video();
		virtual bool create(void* hWindow, unsigned int width, unsigned int height, bool isFullScreen);
		virtual void clear(unsigned int color = 0x00000000);
		virtual void present();
		virtual void render();

		// ¹¤³§º¯Êý
		virtual IShader* createShader();
		virtual ITexture* createTexture();

	protected:
		void cleanup();

	protected:
		void*	mEGLDisplay;
		void*	mEGLSurface;
		void*	mEGLContext;
		unsigned int mClearColor;
	};




}	// namespace easy2d


#endif