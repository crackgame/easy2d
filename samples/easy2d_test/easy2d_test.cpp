// easy2d_test.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "easy2d_test.h"
#include "IApplication.h"
#include "IVideo.h"

using namespace easy2d;

#define MAX_TEX	2

IApplication* g_pApp = NULL;
IVideo* g_pVideo = NULL;
IShader* g_shader = NULL;
ITexture* g_texs[MAX_TEX] = {NULL};

/*
char vShaderStr[] =  
"attribute vec4 vPosition;    \n"
"void main()                  \n"
"{                            \n"
"   gl_Position = vPosition;  \n"
"}                            \n";

char fShaderStr[] =  
"precision mediump float;\n"\
"void main()                                  \n"
"{                                            \n"
"  gl_FragColor = vec4 ( 0.0, 1.0, 0.0, 1.0 );\n"
"}                                            \n";             
*/

char vShaderStr[] =  
"attribute vec4 a_position;   \n"
"attribute vec2 a_texCoord;   \n"
"varying vec2 v_texCoord;     \n"
"void main()                  \n"
"{                            \n"
"   gl_Position = a_position; \n"
"   v_texCoord = a_texCoord;  \n"
"}                            \n";

char fShaderStr[] =  
"precision mediump float;                            \n"
"varying vec2 v_texCoord;                            \n"
"uniform sampler2D s_baseMap;                        \n"
"uniform sampler2D s_lightMap;                       \n"
"void main()                                         \n"
"{                                                   \n"
"  vec4 baseColor;                                   \n"
"  vec4 lightColor;                                  \n"
"                                                    \n"
"  baseColor = texture2D( s_baseMap, v_texCoord );   \n"
"  lightColor = texture2D( s_lightMap, v_texCoord ); \n"
"  gl_FragColor = baseColor * (lightColor + 0.25);   \n"
"}                                                   \n";

/*
float vVertices[] = {  
	0.0f,  0.5f, 0.0f, 
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f };
	*/
	
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
// WORD indices[] = { 0, 1, 2, 0, 2, 3 };


class GameAppListener : public IApplication::IApplicationEventListener
{
public:
	virtual bool onLaunch()
	{
		int width  = g_pApp->getWidth();
		int height = g_pApp->getHeight();
		void* hWindow = g_pApp->getWindowHandle();
		g_pVideo->create(hWindow, width, height, false);

		g_shader = g_pVideo->createShader();
		g_shader->create(vShaderStr, fShaderStr);

		g_texs[0] = g_pVideo->createTexture();
		g_texs[0]->create("basemap.tga");

		g_texs[1] = g_pVideo->createTexture();
		g_texs[1]->create("lightmap.tga");

		positionLoc = g_shader->getAttribLocation("a_position");
		texCoordLoc = g_shader->getAttribLocation("a_texCoord");

		baseMapLoc = g_shader->getUniformLocation ("s_baseMap");
		lightMapLoc = g_shader->getUniformLocation ("s_lightMap");

		return true;
	}

	virtual void onRender()
	{
		g_pVideo->clear();

		g_shader->use();

		// Load the vertex position
		g_shader->setVertexPointer(positionLoc, vVertices, 5 * sizeof(float), IShader::FLOAT_3);
		// Load the texture coordinate
		g_shader->setVertexPointer(texCoordLoc, &vVertices[3], 5 * sizeof(float), IShader::FLOAT_2);

		// Bind the base map
		g_texs[0]->bind(0);

		// Set the base map sampler to texture unit to 0
		g_shader->setUniform1i(baseMapLoc, 0);

		// Bind the light map
		g_texs[1]->bind(1);

		// Set the light map sampler to texture unit 1
		g_shader->setUniform1i(lightMapLoc, 1);

		g_pVideo->render();
		
		g_pVideo->present();
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

protected:
	int positionLoc;
	int texCoordLoc;

	int baseMapLoc;
	int lightMapLoc;

private:
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
	g_pVideo = CreateVideoGLES2();
;

	g_pApp->setTitle( TEXT("OpenglES2-��Ϸ����") );
	g_pApp->setEventListener(&appEvent);
	g_pApp->start(640, 480);


	// �ͷ�������Դ
	ReleaseApplication(&g_pApp);
	ReleaseVideo(&g_pVideo);
	

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
