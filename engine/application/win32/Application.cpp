#include <Windows.h>
#include "../IApplication.h"
#include "Application.h"


namespace easy2d {


	IApplication* CreateApplication()
	{
		return new Application;
	}

	void ReleaseApplication(IApplication** pApp)
	{
		delete *pApp;
		*pApp = NULL;
	}


	Application::Application()
	{
		mAppListener = NULL;
		mWidth = 0;
		mHeight = 0;
		mHwnd = NULL;
		mTitle = TEXT("OpenGL-ES2");
	}

	Application::~Application()
	{

	}

	bool Application::start(unsigned int width, unsigned int height)
	{
		createWindow(width, height);

		// 初始化
		if (mAppListener) {
			if ( !mAppListener->onLaunch() ) {
				OutputDebugString(TEXT("！！！onLaunch failed\n"));
				return false;
			}
		}

		/* Enter event loop */
		int bDone = 0;
		MSG sMessage;
		DWORD lastTick = GetTickCount();
		DWORD curTick = GetTickCount();
		DWORD interval = (DWORD)(1000 / 60);

		while (!bDone) {

			if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE)) {
				if(sMessage.message == WM_QUIT) {
					bDone = 1;
				} else {
					TranslateMessage(&sMessage);
					DispatchMessage(&sMessage);
				}
			}

			// 游戏渲染
			curTick = GetTickCount();
			if (curTick - lastTick > interval) {
				lastTick = curTick;
				if (mAppListener) 
					mAppListener->onRender();
			} else {
				Sleep(1);
			}
		}

		return true;
	}

	void Application::setEventListener(IApplicationEventListener* listener)
	{
		mAppListener = listener;
	}

	void Application::setTitle(const wstring& title)
	{
		mTitle = title;
		if (mHwnd != NULL)
			SetWindowText((HWND)mHwnd, title.c_str());
	}

	void* Application::getWindowHandle()
	{
		return mHwnd;
	}

	unsigned int Application::getWidth()
	{
		return mWidth;
	}

	unsigned int Application::getHeight()
	{
		return mHeight;
	}

	bool Application::createWindow(unsigned int width, unsigned int height)
	{
		WNDCLASS wc;
		RECT wRect;
		HWND sWindow;
		HINSTANCE hInstance;

		wRect.left = 0L;
		wRect.right = (long)width;
		wRect.top = 0L;
		wRect.bottom = (long)height;

		hInstance = GetModuleHandle(NULL);

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)onWndMsg;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = TEXT("OPENGLES2");

		RegisterClass(&wc);

		AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		// 窗口居中
		int screenW = GetSystemMetrics(SM_CXSCREEN);
		int screenH = GetSystemMetrics(SM_CYSCREEN);
		int winPosX = screenW/2 - width/2;
		int winPosY = screenH/2 - height/2;

		sWindow = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, TEXT("OPENGLES2"), mTitle.c_str(), 
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
			winPosX, winPosY, width, height, 
			NULL, NULL, hInstance, NULL);

		ShowWindow(sWindow, SW_SHOW);
		SetForegroundWindow(sWindow);
		SetFocus(sWindow);

		SetWindowLong(sWindow, GWL_USERDATA, (LONG)(this));

		mWidth = width;
		mHeight = height;
		mHwnd = (HWND)sWindow;

		return true;
	}

	LRESULT CALLBACK Application::onWndMsg(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uiMsg) {
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_ACTIVATE:
		case WM_KEYDOWN:
		case WM_KEYUP:
			return 0;

		case WM_SIZE:
			{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);
				Application* pApp = (Application*)(GetWindowLong(hWnd, GWL_USERDATA));
				if (pApp == NULL) break;
				switch (wParam)
				{
				case SIZE_RESTORED:
					if (pApp->mAppListener) pApp->mAppListener->onEnterForeground();
					break;
				case SIZE_MINIMIZED:
					if (pApp->mAppListener) pApp->mAppListener->onEnterBackground();
					break;
				}

				pApp->mAppListener->onScreenSizeChanged(width, height);
			}
			return 0;
		}

		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}


}