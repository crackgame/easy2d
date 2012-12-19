#ifndef	_GLES2_SHADER_H_
#define _GLES2_SHADER_H_

namespace easy2d {

	class GLES2Shader
	{
	public:
		GLES2Shader();
		virtual ~GLES2Shader();
		bool create(void* hWindow, unsigned int width, unsigned int height, bool isFullScreen);
		void clear(unsigned int color = 0x00000000);
		void present();
		void render();

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