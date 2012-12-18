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


			/*
			// 游戏渲染循环在此处
			rotate_matrix(iXangle, 1.0, 0.0, 0.0, aModelView);
			rotate_matrix(iYangle, 0.0, 1.0, 0.0, aRotate);

			multiply_matrix(aRotate, aModelView, aModelView);

			rotate_matrix(iZangle, 0.0, 1.0, 0.0, aRotate);

			multiply_matrix(aRotate, aModelView, aModelView);

			aModelView[14] -= 2.5;

			perspective_matrix(45.0, (double)uiWidth/(double)uiHeight, 0.01, 100.0, aPerspective);
			multiply_matrix(aPerspective, aModelView, aMVP);

			GL_CHECK(glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, aMVP));

			iXangle += 3;
			iYangle += 2;
			iZangle += 1;

			if(iXangle >= 360) iXangle -= 360;
			if(iXangle < 0) iXangle += 360;
			if(iYangle >= 360) iYangle -= 360;
			if(iYangle < 0) iYangle += 360;
			if(iZangle >= 360) iZangle -= 360;
			if(iZangle < 0) iZangle += 360;

			GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
			GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36));

			if (!eglSwapBuffers(sEGLDisplay, sEGLSurface)) {
				printf("Failed to swap buffers.\n");
			}
			*/
	
			
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
			}
			return 0;
		}

		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}


}