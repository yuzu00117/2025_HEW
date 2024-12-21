/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Framework
 * File     : CriSmpFramework_PC.cpp
 * Version  : 0.92LE
 *
 ****************************************************************************/

#include <stdio.h>
#include <windows.h>
#include <cri_le_xpt.h>
#include "CriSmpFramework.h"

/***************************************************************************
 *		定数マクロ
 *		Constant Macros
 ***************************************************************************/
/* ウィンドウクラス名	*/
/* Window class name	*/
#define WNDCLASS_NAME		"CRIWARE Sample Program"
/* ウィンドウタイトル	*/
/* Window title			*/
#define WINDOW_TITLE		"CRIWARE Sample Program"

/* ウィンドウサイズ	*/
/* Window size		*/
#define WINDOW_WIDTH		(960)
#define WINDOW_HEIGHT		(540)

/***************************************************************************
 *		処理マクロ
 *		Process Macros
 ***************************************************************************/
#define SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p) = NULL; } }
#define SAFE_FREE(p)	{ if (p) { free(p); (p) = NULL; } }

/* Warning防止用      */
/* Warning prevention */
#define NOP(x)			(x)

/***************************************************************************
 *      変数定義
 *      Variable Definitions
 ***************************************************************************/
static HWND			g_hwnd = NULL;			// Window handle
static MSG			g_msg;					// Window message
static CriBool			g_is_terminated = FALSE;;

/***************************************************************************
 *		関数定義
 *		Function Definitions
 ***************************************************************************/
extern int __cdecl main();

/* ウィンドウの作成		*/
/* Create the window	*/
static HWND createWindow(HINSTANCE hinst, CriSint32 width, CriSint32 height);

/* ウィンドウの破棄 */
static CriBool destroyWindow(HINSTANCE hinst, HWND hwnd, LPCSTR lpClassName);

/* ウィンドウプロシージャ	*/
/* Window procedure			*/
static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
/* ウィンドウメッセージを処理する	*/
/* Process a window message			*/
static CriBool processMessage(void);

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//	NOP(hPrevInstance);
//	NOP(lpCmdLine);
//	NOP(nShowCmd);
//
//	/* COMの初期化 */
//	CoInitializeEx(NULL, COINIT_MULTITHREADED);
//
//	g_hwnd = createWindow(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT);
//	if (g_hwnd == NULL) {
//		return 1;
//	}
//	main();
//
//	destroyWindow(hInstance, g_hwnd, WNDCLASS_NAME);
//
//	/* COMの終了 */
//	CoUninitialize();
//
//	return (int)g_msg.wParam;
//}


/* ウィンドウの作成		*/
/* Create the window	*/
HWND createWindow(HINSTANCE hinst, CriSint32 width, CriSint32 height)
{
	HWND		hwnd;
	RECT		rc;
	WNDCLASS	wnd_class;

	/* ウィンドウクラスの登録		*/
	/* Register the window class	*/
	wnd_class.style			= 0;
	wnd_class.lpfnWndProc	= wndProc;
	wnd_class.cbClsExtra	= 0;
	wnd_class.cbWndExtra	= 0;
	wnd_class.hInstance		= hinst;
	wnd_class.hIcon			= NULL;
	wnd_class.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wnd_class.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd_class.lpszMenuName	= NULL;
	wnd_class.lpszClassName	= WNDCLASS_NAME;

	if (!RegisterClass(&wnd_class)){
		OutputDebugString("HOGE");
	}

	/* ウィンドウの作成		*/
	/* Create the window	*/
	SetRect(&rc, 0, 0, width, height);
	AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU, FALSE);
	hwnd = CreateWindow(WNDCLASS_NAME, WINDOW_TITLE, WS_CAPTION | WS_SYSMENU,
								 CW_USEDEFAULT, CW_USEDEFAULT,
								 (rc.right-rc.left), (rc.bottom-rc.top), 0L,
								 NULL, hinst, 0L);
	if (hwnd == NULL) {
		return NULL;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return hwnd;
}

CriBool destroyWindow(HINSTANCE hinst, HWND hwnd, LPCSTR lpClassName)
{
	CriBool ret = 0;

	if (hwnd != NULL) {
		ret = DestroyWindow(hwnd);
	}
	if (ret!=FALSE) {
		ret = UnregisterClass(lpClassName, hinst);
	}

	return ret;
}

/* ウィンドウプロシージャ	*/
/* Window procedure			*/
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_CLOSE:
		//DestroyWindow(hwnd);
		g_is_terminated = TRUE;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		HDC hdc;
		PAINTSTRUCT  ps;

		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;
	}
	default:
		return (HRESULT)DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return (HRESULT)DefWindowProc(hwnd, msg, wparam, lparam);
}

/* ウィンドウメッセージを処理する	*/
/* Process a window message			*/
CriBool processMessage(void)
{
	if (PeekMessage(&g_msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (GetMessage(&g_msg, NULL, 0, 0) == 0) {
			return FALSE;
		}
		TranslateMessage(&g_msg);
		DispatchMessage(&g_msg);
	}

	return TRUE;
}

HWND CriSmpFramework_GetWindowHandle(void)
{
	return g_hwnd;
}

/***
*		Class Functions
***/

class CriSmpFrameworkLoc : public CriSmpFramework
{
public:
	virtual CriBool SyncFrame(void);
	virtual void Destroy(void);
	virtual void BeginDraw(void);
	virtual void EndDraw(void);
	virtual CriBool IsTerninated(void);
};

CriSmpFramework* CriSmpFramework::Create(void)
{
	CriSmpFrameworkLoc* smpfw = new CriSmpFrameworkLoc;

	smpfw->idev = CriSmpInputDevice::Create();
	smpfw->vout = CriSmpVideoOutput::Create();
	smpfw->timer = CriSmpTimer::Create();

	return smpfw;
}

void CriSmpFrameworkLoc::Destroy(void)
{
	if (this->timer != NULL) {
		this->timer->Destroy();
		this->timer = NULL;
	}
	if (this->vout != NULL) {
		this->vout->Destroy();
		this->vout = NULL;
	}
	if (this->idev != NULL) {
		this->idev->Destroy();
		this->idev = NULL;
	}
	delete this;
}

CriBool CriSmpFrameworkLoc::SyncFrame(void)
{
	while ( this->timer->GetElapseMsTime() < 1000.0f/60.0f ) {
		if ( processMessage() == FALSE )
			return FALSE;
		Sleep(0);
	}
	this->timer->Start();

	return TRUE;
}

void CriSmpFrameworkLoc::BeginDraw(void)
{
	this->vout->BeginDraw();
}

void CriSmpFrameworkLoc::EndDraw(void)
{
	this->vout->EndDraw();
}

CriBool CriSmpFrameworkLoc::IsTerninated(void)
{
	return g_is_terminated;
}

/***
*		Debug Functions
***/
void CriSmpDebugPrintf(const char *fmt, ...)
{
	va_list	args;
	CriChar8	string[512];

	va_start(args, fmt);
	vsprintf_s((CriChar8 *)string, 512, fmt, args);
	va_end(args);

	OutputDebugString(string);
}

/* --- end of file --- */