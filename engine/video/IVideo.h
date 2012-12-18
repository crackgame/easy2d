#ifndef	_IVIDEO_H_
#define _IVIDEO_H_

namespace easy2d {

class IVideo
{
public:
	enum VideoType
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
	
};


// È«¾Öº¯Êý
extern IVideo* CreateVideoGLES2();
extern void ReleaseVideo(IVideo** pVideo);


}	// namespace easy2d


#endif