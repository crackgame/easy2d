// easy2d_test.cpp : ����Ӧ�ó������ڵ㡣
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
		mTexs[0]->create("res/�·�001.tga");

		mTexs[1] = mVideo->createTexture();
		mTexs[1]->create("res/����001.tga");

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
		stVertex lb;	// ����
		stVertex lt;	// ����
		stVertex rb;	// ����
		stVertex rt;	// ����
	};

	virtual void onRender()
	{
		mVideo->clear(0xFF808080);


		//												2d (opengl)
		stVertex vVertices[] = {
			{
				0.0f,  512.0f, 0.0f,  // Position 0		���£����ϣ�
				0.0f,  0.0f,			// TexCoord 0 
				1.0, 1.0, 1.0, 1.0,		// Color 0
			},

			{
				0.0f,  0.0f, 0.0f,  // Position 1		���ϣ����£�
				0.0f,  1.0f,        // TexCoord 1
				1.0, 1.0, 1.0, 1.0, // Color 1
			},

			{
				512.0f,  512.0f, 0.0f,  // Position 3	���£����ϣ�
				1.0f,  0.0f,         // TexCoord 3
				1.0, 1.0, 1.0, 1.0, // Color 2
			},

			{
				512.0f, 0.0f, 0.0f,  // Position 2		���ϣ����£�
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
	g_pApp->setTitle( TEXT("OpenglES2-��Ϸ����") );
	g_pApp->setEventListener(&appEvent);
	g_pApp->start(800, 600);


	// �ͷ�������Դ
	ReleaseApplication(&g_pApp);
	ReleaseVideo(&appEvent.mVideo);

	return 0;
}







//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//#define MAX_LOADSTRING 100
//
//// ȫ�ֱ���:
//HINSTANCE hInst;								// ��ǰʵ��
//TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
//TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
//
//// �˴���ģ���а����ĺ�����ǰ������:
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

 //	// TODO: �ڴ˷��ô��롣
	//MSG msg;
	//HACCEL hAccelTable;

	//// ��ʼ��ȫ���ַ���
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_EASY2D_TEST, szWindowClass, MAX_LOADSTRING);
	//MyRegisterClass(hInstance);

	//// ִ��Ӧ�ó����ʼ��:
	//if (!InitInstance (hInstance, nCmdShow))
	//{
	//	return FALSE;
	//}

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EASY2D_TEST));

	//// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
//		// �����˵�ѡ��:
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
//		// TODO: �ڴ���������ͼ����...
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
//// �����ڡ������Ϣ�������
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
