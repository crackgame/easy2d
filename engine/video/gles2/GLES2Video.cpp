#include "../IVideo.h"

#include "kazmath/kazmath.h"
#include "kazmath/matrix.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"

#include "GLES2Video.h"
#include "GLES2Shader.h"
#include "GLES2Texture.h"

#include <stdio.h>
#include <stdlib.h>


namespace easy2d {

#define GL_CHECK(x) \
	x; \
	{ \
		GLenum glError = glGetError(); \
		if(glError != GL_NO_ERROR) { \
			fprintf(stderr, "glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__); \
			system("pause");	\
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
		// 初始化EGL相关数据
#ifdef _WIN32
		HDC			hDisplay = GetDC(NULL);
#else
		Display*	hDisplay = XOpenDisplay(NULL);
#endif

		if (hDisplay == NULL)
			return false;


		/* EGL Configuration */

		EGLint aEGLAttributes[] = {
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_DEPTH_SIZE, 16,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE
		};

		EGLint aEGLContextAttributes[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};

		EGLConfig	aEGLConfigs[1];
		EGLint		cEGLConfigs;


		mEGLDisplay = EGL_CHECK(eglGetDisplay(hDisplay));

		EGL_CHECK(eglInitialize(mEGLDisplay, NULL, NULL));
		EGL_CHECK(eglChooseConfig(mEGLDisplay, aEGLAttributes, aEGLConfigs, 1, &cEGLConfigs));

		if (cEGLConfigs == 0) {
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
		GL_CHECK( glEnable(GL_TEXTURE_2D) );
		//GL_CHECK( glEnable(GL_CULL_FACE) );
		//GL_CHECK( glEnable(GL_DEPTH_TEST) );	// 默认不开深度测试，开了会有显示问题
		GL_CHECK( glEnable(GL_BLEND) );
		GL_CHECK( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
		//GL_CHECK( glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA) );
		

		resize(width, height);

		return true;
	}

	void GLES2Video::resize(unsigned int width, unsigned int height)
	{
		// 最小化时的特殊处理
		if (width == 0 || height == 0)
			return;

		const float contentScal = 1.0f;

		GL_CHECK( glViewport (0, 0, width, height) );

		// 设置默认变换矩阵
		kmGLMatrixMode(KM_GL_PROJECTION);
		kmGLLoadIdentity();
		kmMat4 orthoMatrix;
		//kmMat4OrthographicProjection(&orthoMatrix, 0, width / contentScal, 0, height / contentScal, -1024, 1024 );	// 正面
		kmMat4OrthographicProjection(&orthoMatrix, 0, width / contentScal, height / contentScal, 0, -1024, 1024 );		// 反面
		kmGLMultMatrix(&orthoMatrix);
		kmGLMatrixMode(KM_GL_MODELVIEW);
		kmGLLoadIdentity();
	}

	void GLES2Video::clear(unsigned int color /* = 0x00000000 */)
	{
		if (mClearColor != color) {
			mClearColor = color;
			GLclampf magicNum = 1.0f / 255.0f;
			GLclampf a = ((color & 0xFF000000) >> 24) * magicNum;
			GLclampf r = ((color & 0x00FF0000) >> 16) * magicNum;
			GLclampf g = ((color & 0x0000FF00) >> 8) * magicNum;
			GLclampf b = ((color & 0x000000FF) >> 0) * magicNum;
			GL_CHECK( glClearColor(r, g, b, a) );
		}

		GL_CHECK( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
	}

	void GLES2Video::present()
	{
		EGL_CHECK( eglSwapBuffers(mEGLDisplay, mEGLSurface) );
	}

	void GLES2Video::render()
	{
		
		//glBlendFunc(1, 771);

		// 测试的代码
		//glDrawArrays ( GL_TRIANGLES, 0, 3 );

		//index方式
		//GLushort indices[] = { 0, 1, 2 };
		//glDrawElements ( GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices );

		//GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
		GLushort indices[] = { 0, 1, 2, 2, 1, 3 };
		int count = sizeof(indices)/sizeof(indices[0]);
		for (int i=0; i<1; i++) {
			glDrawElements ( GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indices );
		}

		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//Sleep(1);
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