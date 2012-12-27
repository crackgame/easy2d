#ifndef	_ISHADER_H_
#define _ISHADER_H_

namespace easy2d {

class IShader
{
public:
	enum EShaderType
	{
		ShaderVertex,	// ����shader(���㴦��)
		ShaderFragment,	// Ƭ��shader(���ش���)
	};

	enum EVertexFormat
	{
		// ��������
		FLOAT_1,
		FLOAT_2,
		FLOAT_3,
		FLOAT_4,
		// �ֽ�����
		BYTES_1,
		BYTES_2,
		BYTES_3,
		BYTES_4,
	};

public:
	virtual ~IShader(){}
	// ����shader
	// vShaderSource ����shaderԴ��
	// fShaderSource Ƭ��shaderԴ��
	virtual bool create(const char* vertexSource, const char* fragmentSource) = 0;
	virtual int getAttribLocation(const char* name) = 0;
	virtual int getUniformLocation(const char* name) = 0;
	virtual bool setVertexPointer(unsigned int index, const void* ptr, int stride, EVertexFormat foramt) = 0;
	virtual bool setUniform1i(int location, int x) = 0;
	virtual bool setUniform1f(int location, float x) = 0;
	virtual bool setUniformMatrix4fv(int location, const float* values) = 0;
	virtual void use() = 0;
};



}	// namespace easy2d


#endif