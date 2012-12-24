#include "../IVideo.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"

#include "GLES2Video.h"
#include "GLES2Shader.h"
#include "GLES2Texture.h"

#include <stdio.h>
#include <stdlib.h>


namespace easy2d {


	/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
	/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
	/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
	/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
	/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8



	static void printGLString(const char *name, GLenum s) {
		const char *v = (const char *) glGetString(s);
		printf("GL %s = %s\n", name, v);
	}

	static void checkGlError(const char* op) {
		for (GLint error = glGetError(); error; error = glGetError()) {
				printf("after %s() glError (0x%x)\n", op, error);
		}
	}



#define GL_CHECK(x) \
	x; \
	{ \
		GLenum glError = glGetError(); \
		if(glError != GL_NO_ERROR) { \
			fprintf(stderr, "glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__); \
			exit(1); \
		} \
	}

#define EGL_CHECK(x) \
	x; \
	{ \
		EGLint eglError = eglGetError(); \
		if(eglError != EGL_SUCCESS) { \
			fprintf(stderr, "eglGetError() = %i (0x%.8x) at line %i\n", eglError, eglError, __LINE__); \
			exit(1); \
		} \
	}


	// 全局函数
	IVideo* CreateVideoGLES2()
	{
		return new GLES2Video;
	}

	void ReleaseVideo(IVideo** pVideo)
	{
		delete *pVideo;
		*pVideo = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	
	GLES2Video::GLES2Video()
	{
		mWidth = 0;
		mHeight = 0;
		mClearColor = 0;
		mEGLDisplay = NULL;
		mEGLSurface = NULL;
		mEGLContext = NULL;
	}

	GLES2Video::~GLES2Video()
	{
		cleanup();
	}

	void GLES2Video::cleanup()
	{
		/* Cleanup shaders */
		GL_CHECK(glUseProgram(0));

		/* EGL clean up */
		EGL_CHECK(eglMakeCurrent(mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
		EGL_CHECK(eglDestroySurface(mEGLDisplay, mEGLSurface));
		EGL_CHECK(eglDestroyContext(mEGLDisplay, mEGLContext));
		EGL_CHECK(eglTerminate(mEGLDisplay));

#ifndef _WIN32
		/* X windows clean up */
		XDestroyWindow(hDisplay, hWindow);
		XFreeColormap(hDisplay, colormap);
		XFree(pVisual);
		XCloseDisplay(hDisplay);
#endif
	}

	bool GLES2Video::create(void* hWindow, unsigned int width, unsigned int height, bool isFullScreen)
	{
		printGLString("Version", GL_VERSION);
		printGLString("Vendor", GL_VENDOR);
		printGLString("Renderer", GL_RENDERER);
		printGLString("Extensions", GL_EXTENSIONS);


		// 初始化EGL相关数据
#ifdef _WIN32
		HDC			hDisplay = GetDC(NULL);
#else
		Display*	hDisplay = XOpenDisplay(NULL);
#endif

		if (hDisplay == NULL)
			return false;

		EGLint aEGLAttributes[] = {

			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_DEPTH_SIZE, 16,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE
		};

		///* EGL Configuration */
		//int flags = ES_WINDOW_RGB;
		//EGLint aEGLAttributes[] = {
		//	EGL_RED_SIZE,       5,
		//	EGL_GREEN_SIZE,     6,
		//	EGL_BLUE_SIZE,      5,
		//	EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
		//	EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
		//	EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
		//	EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
		//	EGL_NONE
		//};

		EGLint aEGLContextAttributes[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE, EGL_NONE
		};
		
		EGLConfig	aEGLConfigs[1];
		EGLint		numConfigs;
		EGLint		majorVersion;
		EGLint		minorVersion;
		
		mEGLDisplay = EGL_CHECK(eglGetDisplay(hDisplay));

		EGL_CHECK(eglInitialize(mEGLDisplay, &majorVersion, &minorVersion));
		EGL_CHECK(eglGetConfigs(mEGLDisplay, NULL, 0, &numConfigs));
		EGL_CHECK(eglChooseConfig(mEGLDisplay, aEGLAttributes, aEGLConfigs, 1, &numConfigs));
		if (numConfigs == 0) { 
			printf("No EGL configurations were returned.\n");
			return false;
		}

		mEGLSurface = EGL_CHECK(eglCreateWindowSurface(mEGLDisplay, aEGLConfigs[0], (EGLNativeWindowType)hWindow, NULL));
		if (mEGLSurface == EGL_NO_SURFACE) {
			printf("Failed to create EGL surface.\n");
			return false;
		}

		mEGLContext = EGL_CHECK(eglCreateContext(mEGLDisplay, aEGLConfigs[0], EGL_NO_CONTEXT, aEGLContextAttributes));
		if (mEGLContext == EGL_NO_CONTEXT) {
			printf("Failed to create EGL context.\n");
			return false;
		}

		EGL_CHECK(eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext));


		// 初始化gles2相关函数
		GL_CHECK(glEnable(GL_CULL_FACE));
		GL_CHECK(glEnable(GL_DEPTH_TEST));
		GL_CHECK(glViewport (0, 0, width, height));

		mWidth = width;
		mHeight = height;

		return true;
	}

	void GLES2Video::clear(unsigned int color /* = 0x00000000 */)
	{
		glViewport ( 0, 0, mWidth, mHeight );

		if (mClearColor != color) {
			mClearColor = color;
			GLclampf magicNum = 1.0f / 255.0f;
			GLclampf a = ((color & 0xFF000000) >> 24) * magicNum;
			GLclampf r = ((color & 0x00FF0000) >> 16) * magicNum;
			GLclampf g = ((color & 0x0000FF00) >> 8) * magicNum;
			GLclampf b = ((color & 0x000000FF) >> 0) * magicNum;
			GL_CHECK( glClearColor(r, g, b, a) );
		}

		//GL_CHECK( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
		GL_CHECK( glClear(GL_COLOR_BUFFER_BIT) );
	}

	void GLES2Video::present()
	{
		EGL_CHECK( eglSwapBuffers(mEGLDisplay, mEGLSurface) );
	}

	void GLES2Video::render()
	{
		GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f };

		// Load the vertex data
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
		glEnableVertexAttribArray ( 0 );

		glDrawArrays ( GL_TRIANGLES, 0, 3 );
		

		// 测试的代码
		//glDrawArrays ( GL_TRIANGLES, 0, 3 );

		//GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
		//GL_CHECK( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices) );
	}


	//////////////////////////////////////////////////////////////////////////
	// 创建函数

	IShader* GLES2Video::createShader()
	{
		return new GLES2Shader;
	}

	ITexture* GLES2Video::createTexture()
	{
		return new GLES2Texture;
	}

}