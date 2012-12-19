#include "GLES2Shader.h"

#include <stdio.h>
#include <stdlib.h>

#include "GLES2/gl2.h"
#include "EGL/egl.h"

namespace easy2d {

#define GL_CHECK(x) \
	x; \
	{ \
		GLenum glError = glGetError(); \
		if(glError != GL_NO_ERROR) { \
			fprintf(stderr, "glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__); \
			system("pause"); \
			exit(1); \
		} \
	}

#define EGL_CHECK(x) \
	x; \
	{ \
		EGLint eglError = eglGetError(); \
		if(eglError != EGL_SUCCESS) { \
			fprintf(stderr, "eglGetError() = %i (0x%.8x) at line %i\n", eglError, eglError, __LINE__); \
			system("pause"); \
			exit(1); \
		} \
	}

	//////////////////////////////////////////////////////////////////////////

	// 临时测试
	GLuint LoadShader ( GLenum type, const char *shaderSrc )
	{
		GLuint shader;
		GLint compiled;

		// Create the shader object
		shader = glCreateShader ( type );

		if ( shader == 0 )
			return 0;

		// Load the shader source
		glShaderSource ( shader, 1, &shaderSrc, NULL );

		// Compile the shader
		glCompileShader ( shader );

		// Check the compile status
		glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

		if ( !compiled ) 
		{
			GLint infoLen = 0;

			glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

			if ( infoLen > 1 )
			{
				char* infoLog = new char[infoLen];

				glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
				printf( "Error compiling shader:\n%s\n", infoLog );            

				delete[] infoLog;
			}

			glDeleteShader ( shader );
			return 0;
		}

		return shader;

	}


	GLuint programObject = 0;

	int Init ()
	{
		char vShaderStr[] =  
			"attribute vec4 vPosition;    \n"
			"void main()                  \n"
			"{                            \n"
			"   gl_Position = vPosition;  \n"
			"}                            \n";

		char fShaderStr[] =  
			"precision mediump float;\n"\
			"void main()                                  \n"
			"{                                            \n"
			"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
			"}                                            \n";

		GLuint vertexShader;
		GLuint fragmentShader;
		//GLuint programObject;
		GLint linked;

		// Load the vertex/fragment shaders
		vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
		fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

		// Create the program object
		programObject = glCreateProgram ( );

		if ( programObject == 0 )
			return 0;

		glAttachShader ( programObject, vertexShader );
		glAttachShader ( programObject, fragmentShader );

		// Bind vPosition to attribute 0   
		glBindAttribLocation ( programObject, 0, "vPosition" );

		// Link the program
		glLinkProgram ( programObject );

		// Check the link status
		glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

		if ( !linked ) 
		{
			GLint infoLen = 0;

			glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

			if ( infoLen > 1 )
			{
				char* infoLog = new char[infoLen];

				glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
				printf( "Error linking program:\n%s\n", infoLog );            

				delete[] infoLog;
			}

			glDeleteProgram ( programObject );
			return FALSE;
		}

		return TRUE;
	}


	///
	// Draw a triangle using the shader pair created in Init()
	//
	void Draw()
	{
		GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f };

		// Use the program object
		glUseProgram ( programObject );

		// Load the vertex data
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
		glEnableVertexAttribArray ( 0 );

		glDrawArrays ( GL_TRIANGLES, 0, 3 );
	}

	//////////////////////////////////////////////////////////////////////////

	GLES2Shader::GLES2Shader()
	{
		mClearColor = 0;
		mEGLDisplay = NULL;
		mEGLSurface = NULL;
		mEGLContext = NULL;
	}

	GLES2Shader::~GLES2Shader()
	{
		cleanup();
	}

	void GLES2Shader::cleanup()
	{
		/* Cleanup shaders */
		GL_CHECK(glUseProgram(0));
		//GL_CHECK(glDeleteShader(uiVertShader));
		//GL_CHECK(glDeleteShader(uiFragShader));
		//GL_CHECK(glDeleteProgram(uiProgram));

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

	bool GLES2Shader::create(void* hWindow, unsigned int width, unsigned int height, bool isFullScreen)
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
		GL_CHECK(glEnable(GL_CULL_FACE));
		GL_CHECK(glEnable(GL_DEPTH_TEST));
		GL_CHECK(glViewport (0, 0, width, height));

		Init();

		return true;
	}

	void GLES2Shader::clear(unsigned int color /* = 0x00000000 */)
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

		GL_CHECK( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
	}

	void GLES2Shader::present()
	{
		EGL_CHECK( eglSwapBuffers(mEGLDisplay, mEGLSurface) );
	}

	void GLES2Shader::render()
	{
		// 测试的代码
		Draw();
	}


}