#ifndef	_GLES2_SHADER_H_
#define _GLES2_SHADER_H_

namespace easy2d {

	class GLES2Shader : public IShader
	{
	public:
		GLES2Shader();
		virtual ~GLES2Shader();
		virtual bool create(const char* vertexSource, const char* fragmentSource);
		virtual bool bindAttrib(unsigned int index, const char* name);
		virtual bool setVertexPointer(unsigned int index, const void* ptr, int stride, EVertexFormat foramt);
		virtual void use();

	protected:
		GLuint loadShader(GLenum type, const char *shaderSrc);
		bool checkProgramInfoLog();
		bool transOpenGLParams();

	protected:
		GLuint mProgramObject;
		GLuint mVertexShader;
		GLuint mFragmentShader;
	};




}	// namespace easy2d


#endif