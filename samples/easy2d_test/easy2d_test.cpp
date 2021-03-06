// easy2d_test.cpp : 定义应用程序的入口点。
//


#include "kazmath/matrix.h"
#include "kazmath/kazmath.h"

#include "stdafx.h"
#include "easy2d_test.h"


#include "IApplication.h"
#include "IVideo.h"
using namespace easy2d;

#include <string>
using namespace std;



#define MAX_TEX	2
IApplication* g_pApp = NULL;

string loadShaderFile(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL)
		return "";

	fseek(file, 0, SEEK_END);
	long filelen = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buff = new char[filelen+1];
	memset(buff, 0, filelen+1);
	fread(buff, filelen, 1, file);
	buff[filelen] = '\0';

	fclose(file);

	string strSource = buff;

	delete[] buff;

	return strSource;
}


class GameAppListener : public IApplication::IApplicationEventListener
{
public:
	virtual bool onLaunch()
	{
		string strVertexSrc = loadShaderFile("res/default.vert");
		string strFragmentSrc = loadShaderFile("res/default.frag");

		mVideo = CreateVideoGLES2();
		int width  = g_pApp->getWidth();
		int height = g_pApp->getHeight();
		void* hWindow = g_pApp->getWindowHandle();
		mVideo->create(hWindow, width, height, false);

		mShader = mVideo->createShader();
		mShader->create(strVertexSrc.c_str(), strFragmentSrc.c_str());

		mTexs[0] = mVideo->createTexture();
		mTexs[0]->create("res/衣服001.tga");

		mTexs[1] = mVideo->createTexture();
		mTexs[1]->create("res/武器001.tga");

		mMVPMatrixLoc = mShader->getUniformLocation("u_MVPMatrix");
		mTextureLoc   = mShader->getUniformLocation("s_texture");

		mPositionLoc  = mShader->getAttribLocation("a_position");
		mTexCoordLoc  = mShader->getAttribLocation("a_texCoord");
		mColorLoc	  = mShader->getAttribLocation("a_color");

		return true;
	}

	struct stVertex
	{
		float x, y, z;
		float u, v;
		float r, g, b, a;
	};

	struct stQuad
	{
		stVertex lb;	// 左下
		stVertex lt;	// 左上
		stVertex rb;	// 右下
		stVertex rt;	// 右上
	};

	virtual void onRender()
	{
		mVideo->clear(0xFF808080);


		//												2d (opengl)
		stVertex vVertices[] = {
			{
				0.0f,  512.0f, 0.0f,  // Position 0		左下（左上）
				0.0f,  0.0f,			// TexCoord 0 
				1.0, 1.0, 1.0, 1.0,		// Color 0
			},

			{
				0.0f,  0.0f, 0.0f,  // Position 1		左上（左下）
				0.0f,  1.0f,        // TexCoord 1
				1.0, 1.0, 1.0, 1.0, // Color 1
			},

			{
				512.0f,  512.0f, 0.0f,  // Position 3	右下（右上）
				1.0f,  0.0f,         // TexCoord 3
				1.0, 1.0, 1.0, 1.0, // Color 2
			},

			{
				512.0f, 0.0f, 0.0f,  // Position 2		右上（右下）
				1.0f,  1.0f,        // TexCoord 2
				1.0, 1.0, 1.0, 1.0, // Color 3
			},
		};

		mShader->use();

		// Load the vertex data
		mShader->setVertexPointer(mPositionLoc, &vVertices[0].x, sizeof(stVertex), IShader::FLOAT_3);

		// Load the texcoord data
		mShader->setVertexPointer(mTexCoordLoc, &vVertices[0].u, sizeof(stVertex), IShader::FLOAT_2);

		// Load the color data
		mShader->setVertexPointer(mColorLoc,	&vVertices[0].r, sizeof(stVertex), IShader::FLOAT_4);


		kmMat4 matrixP;
		kmMat4 matrixMV;
		kmMat4 matrixMVP;
		kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
		kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
		kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
		mShader->setUniformMatrix4fv(mMVPMatrixLoc, matrixMVP.mat);


		mTexs[0]->bind(0);
		mShader->setUniform1i(mTextureLoc, 0);
		mVideo->render();

		mTexs[1]->bind(0);
		mShader->setUniform1i(mTextureLoc, 0);
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
		mVideo->resize(width, height);
		return;
	}

public:
	IVideo* mVideo;
	IShader* mShader;
	ITexture* mTexs[MAX_TEX];

	int mMVPMatrixLoc;
	int mPositionLoc;
	int mTexCoordLoc;
	int mTextureLoc;
	int mColorLoc;
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
	g_pApp->start(800, 600);


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
