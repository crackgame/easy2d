#include "../IShader.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"

#include "GLES2Shader.h"

#include <stdio.h>
#include <stdlib.h>



namespace easy2d {

	GLES2Shader::GLES2Shader()
	{
		mProgramObject = 0;
		mVertexShader = 0;
		mFragmentShader = 0;
	}

	GLES2Shader::~GLES2Shader()
	{
		if (mProgramObject > 0)
			glDeleteShader(mProgramObject);
	}

	GLuint GLES2Shader::loadShader(GLenum type, const char *shaderSrc)
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

	bool GLES2Shader::checkProgramInfoLog()
	{
		GLint linked;

		// Check the link status
		glGetProgramiv ( mProgramObject, GL_LINK_STATUS, &linked );

		if ( !linked ) 
		{
			GLint infoLen = 0;

			glGetProgramiv ( mProgramObject, GL_INFO_LOG_LENGTH, &infoLen );

			if ( infoLen > 1 )
			{
				char* infoLog = new char[infoLen];

				glGetProgramInfoLog ( mProgramObject, infoLen, NULL, infoLog );
				printf( "Error linking program:\n%s\n", infoLog );            

				delete[] infoLog;
			}

			glDeleteProgram ( mProgramObject );
			return false;
		}

		return true;
	}

	bool GLES2Shader::create(const char* vertexSource, const char* fragmentSource)
	{
		mVertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
		if (mVertexShader <= 0)
			return false;

		mFragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
		if (mFragmentShader <= 0)
			return false;

		mProgramObject = glCreateProgram();
		if (mProgramObject == 0)
			return false;

		glAttachShader (mProgramObject, mVertexShader);
		glAttachShader (mProgramObject, mFragmentShader);

		// Link the program
		glLinkProgram ( mProgramObject );

		if (!checkProgramInfoLog())
			return false;

		if (mVertexShader > 0)
			glDeleteShader(mVertexShader);

		if (mFragmentShader > 0)
			glDeleteShader(mFragmentShader);

		return true;
	}

	int GLES2Shader::getAttribLocation(const char* name)
	{
		return glGetAttribLocation(mProgramObject, name);
	}

	int GLES2Shader::getUniformLocation(const char* name)
	{
		return glGetUniformLocation(mProgramObject, name);
	}

	bool GLES2Shader::setVertexPointer(unsigned int index, const void* ptr, int stride, EVertexFormat foramt)
	{
		GLint size = 0;
		GLenum type = 0;
		switch (foramt)
		{
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		case FLOAT_4:
			size = foramt - FLOAT_1 + 1;
			type = GL_FLOAT;
			break;
		case BYTES_1:
		case BYTES_2:
		case BYTES_3:
		case BYTES_4:
			size = foramt - BYTES_1 + 1;
			type = GL_BYTE;
			break;
		}


		// Load the vertex data
		glVertexAttribPointer ( index, size, type, GL_FALSE, stride, ptr );
		glEnableVertexAttribArray ( index );
		return true;
	}

	bool GLES2Shader::setUniform1i(int location, int x)
	{
		glUniform1i (location, x);
		return true;
	}

	void GLES2Shader::use()
	{
		glUseProgram ( mProgramObject );
	}


}