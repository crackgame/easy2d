#ifndef	_IAPPLICATION_H_
#define _IAPPLICATION_H_

#include <string>
using namespace std;

namespace easy2d {

	class IApplication
	{
	public:
		class IApplicationEventListener
		{
		public:
			virtual ~IApplicationEventListener(){}
			virtual bool onLaunch() = 0;
			virtual void onRender() = 0;
			virtual void onEnterBackground() = 0;
			virtual void onEnterForeground() = 0;
			virtual void onScreenSizeChanged(int width, int height) = 0;
		};

	public:
		virtual ~IApplication(){}
		virtual bool start(unsigned int width, unsigned int height) = 0;
		virtual void setEventListener(IApplicationEventListener* listener) = 0;
		virtual void setTitle(const wstring& title) = 0;
		virtual void* getWindowHandle() = 0;
		virtual unsigned int getWidth() = 0;
		virtual unsigned int getHeight() = 0;
	};


	// 创建应用对象
	IApplication* CreateApplication();
	void ReleaseApplication(IApplication** pApp);

}	// namespace easy2d



#endif