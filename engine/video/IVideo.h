#ifndef	_IVIDEO_H_
#define _IVIDEO_H_
#include "IShader.h"
#include "ITexture.h"

namespace easy2d {


class IVideo
{
public:
	enum EVideoType
	{
		VideoGLES2,
		VideoD3D9,
	};

public:
	virtual ~IVideo(){}
	virtual bool create(void* hWindow, unsigned int width, unsigned int height, bool isFullScreen) = 0;
	virtual void clear(unsigned int color = 0x00000000) = 0;
	virtual void present() = 0;
	virtual void render() = 0;
	virtual void resize(unsigned int width, unsigned int height) = 0;

	// 工厂函数
	virtual IShader* createShader() = 0;
	virtual ITexture* createTexture() = 0;

	
};


// 全局函数
extern IVideo* CreateVideoGLES2();
extern void ReleaseVideo(IVideo** pVideo);


}	// namespace easy2d


#endif