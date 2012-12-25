// easy2d_test.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "easy2d_test.h"
#include "IApplication.h"
#include "IVideo.h"
using namespace easy2d;


#define MAX_TEX	2
IApplication* g_pApp = NULL;


class GameAppListener : public IApplication::IApplicationEventListener
{
public:
	virtual bool onLaunch()
	{
		/*
		char vShaderStr[] =  
			"attribute vec4 a_position;    \n"
			"void main()                  \n"
			"{                            \n"
			"   gl_Position = a_position;  \n"
			"}                            \n";

		char fShaderStr[] =  
			"precision mediump float;\n"\
			"void main()                                  \n"
			"{                                            \n"
			"  gl_FragColor = vec4 ( 1.0, 1.0, 0.0, 0.0 );\n"
			"}                                            \n";
		*/

		char vShaderStr[] =  
			"attribute vec4 a_position;    \n"
			"attribute vec2 a_texCoord;   \n"
			"varying vec2 v_texCoord;     \n"
			"void main()                  \n"
			"{                            \n"
			"   gl_Position = a_position;  \n"
			"   v_texCoord = a_texCoord;  \n"
			"}                            \n";

		char fShaderStr[] =  
			"precision mediump float;					  \n"
			"varying vec2 v_texCoord;                     \n"
			"uniform sampler2D s_baseMap;                 \n"
			"void main()                                  \n"
			"{                                            \n"
			"  vec4 baseColor;                            \n"

			"  baseColor = texture2D( s_baseMap, v_texCoord );   \n"
			"  gl_FragColor = baseColor * vec4 ( 1.0, 1.0, 0.0, 0.0 );\n"
//			"  gl_FragColor = baseColor;					\n"
			"}                                            \n";

		mVideo = CreateVideoGLES2();
		int width  = g_pApp->getWidth();
		int height = g_pApp->getHeight();
		void* hWindow = g_pApp->getWindowHandle();
		mVideo->create(hWindow, width, height, false);

		mShader = mVideo->createShader();
		mShader->create(vShaderStr, fShaderStr);

		mTexs[0] = mVideo->createTexture();
		mTexs[0]->create("basemap.tga");

		mPositionLoc = mShader->getAttribLocation("a_position");
		mTexCoordLoc = mShader->getAttribLocation("a_texCoord");

		mBaseMapLoc = mShader->getUniformLocation("s_baseMap");

		return true;
	}

	virtual void onRender()
	{
		float vVertices[] = {
			-0.5f,  0.5f, 0.0f,  // Position 0
			0.0f,  0.0f,        // TexCoord 0 
			-0.5f, -0.5f, 0.0f,  // Position 1
			0.0f,  1.0f,        // TexCoord 1
			0.5f, -0.5f, 0.0f,  // Position 2
			1.0f,  1.0f,        // TexCoord 2
			0.5f,  0.5f, 0.0f,  // Position 3
			1.0f,  0.0f         // TexCoord 3
		};

		mVideo->clear(0xFF808080);

		mShader->use();

		// Load the vertex data
		mShader->setVertexPointer(mPositionLoc, vVertices, 5 * sizeof(float), IShader::FLOAT_3);

		// Load the texcoord data
		mShader->setVertexPointer(mTexCoordLoc, &vVertices[3], 5 * sizeof(float), IShader::FLOAT_2);

		mVideo->render();

		mVideo->present();
		return;
	}

	virtual void onEnterBackground()
	{

		return;
	}

	virtual void onEnterForeground()
	{
		return;
	}

	virtual void onScreenSizeChanged(int width, int height)
	{

		return;
	}

public:
	IVideo* mVideo;
	IShader* mShader;
	ITexture* mTexs[MAX_TEX];

	int mPositionLoc;
	int mTexCoordLoc;

	int mBaseMapLoc;
	int mLightMapLoc;
};


#if 0
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
#else
int main()
{
#endif

	GameAppListener appEvent;

	g_pApp = CreateApplication();
	g_pApp->setTitle( TEXT("OpenglES2-游戏窗口") );
	g_pApp->setEventListener(&appEvent);
	g_pApp->start(320, 240);


	// 释放所有资源
	ReleaseApplication(&g_pApp);
	ReleaseVideo(&appEvent.mVideo);
	

	return 0;
}







//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//#define MAX_LOADSTRING 100
//
//// 全局变量:
//HINSTANCE hInst;								// 当前实例
//TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
//TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
//
//// 此代码模块中包含的函数的前向声明:
//ATOM				MyRegisterClass(HINSTANCE hInstance);
//BOOL				InitInstance(HINSTANCE, int);
//LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//int APIENTRY _tWinMain(HINSTANCE hInstance,
//                     HINSTANCE hPrevInstance,
//                     LPTSTR    lpCmdLine,
//                     int       nCmdShow)
//{
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);

 //	// TODO: 在此放置代码。
	//MSG msg;
	//HACCEL hAccelTable;

	//// 初始化全局字符串
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_EASY2D_TEST, szWindowClass, MAX_LOADSTRING);
	//MyRegisterClass(hInstance);

	//// 执行应用程序初始化:
	//if (!InitInstance (hInstance, nCmdShow))
	//{
	//	return FALSE;
	//}

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EASY2D_TEST));

	//// 主消息循环:
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	//return (int) msg.wParam;

//	return 0;
//}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc	= WndProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EASY2D_TEST));
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
//	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EASY2D_TEST);
//	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassEx(&wcex);
//}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // 将实例句柄存储在全局变量中
//
//   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//	switch (message)
//	{
//	case WM_COMMAND:
//		wmId    = LOWORD(wParam);
//		wmEvent = HIWORD(wParam);
//		// 分析菜单选择:
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		// TODO: 在此添加任意绘图代码...
//		EndPaint(hWnd, &ps);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//// “关于”框的消息处理程序。
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}
