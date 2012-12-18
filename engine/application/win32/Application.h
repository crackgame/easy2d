#ifndef	_WIN32_APPLICATION_H_
#define _WIN32_APPLICATION_H_

namespace easy2d
{
	class Application : public IApplication
	{
	public:
		Application();
		virtual ~Application();

		virtual bool start(unsigned int width, unsigned int height);
		virtual void setEventListener(IApplicationEventListener* listener);
		virtual void setTitle(const wstring& title);
		virtual void* getWindowHandle();
		virtual unsigned int getWidth();
		virtual unsigned int getHeight();

	protected:
		bool createWindow(unsigned int width, unsigned int height);
		static LRESULT CALLBACK onWndMsg(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

	protected:
		IApplicationEventListener* mAppListener;
		unsigned int mWidth;
		unsigned int mHeight;
		void* mHwnd;
		wstring mTitle;
	};





}	// namespace



#endif