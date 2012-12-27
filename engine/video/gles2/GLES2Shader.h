#ifndef	_GLES2_SHADER_H_
#define _GLES2_SHADER_H_

namespace easy2d {

	class GLES2Shader : public IShader
	{
	public:
		GLES2Shader();
		virtual ~GLES2Shader();
		virtual bool create(const char* vertexSource, const char* fragmentSource);
		virtual int getAttribLocation(const char* name);
		virtual int getUniformLocation(const char* name);
		virtual bool setVertexPointer(unsigned int index, const void* ptr, int stride, EVertexFormat foramt);
		virtual bool setUniform1i(int location, int x);
		virtual bool setUniform1f(int location, float x);
		virtual bool setUniformMatrix4fv(int location, const float* values);
		virtual void use();

	protected:
		GLuint loadShader(GLenum type, const char *shaderSrc);
		bool checkProgramInfoLog();
		bool transOpenGLParams();

	protected:
		GLuint mProgramObject;
	};




}	// namespace easy2d


#endif