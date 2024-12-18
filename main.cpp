//-----------------------------------------------------------------------------------------------------
// #name main.cpp
// #description main 
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
//          ・基本的に授業通りのテンプレート引っ張ってきた
//          
//----------------------------------------------------------------------------------------------------

#include <Windows.h>
#include "renderer.h"
#include "sprite.h"
#include "keyboard.h"
#include "sound.h"
#include"game.h"
#include"scene.h"



//ライブラリのリンク
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//マクロ定義
#define CLASS_NAME "GameWindow"
#define WINDOW_CAPTION "このウィンドウを消すとプログラム終了"

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



//グローバル変数
HWND hWnd;

#ifdef _DEBUG
int g_CountFPS;
char g_DebugStr[2048] = WINDOW_CAPTION;
#endif//_DEBUG


//メイン
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);




	Game& game = Game::GetInstance();

	

	//ウィンドウクラスの登録
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	RegisterClass(&wc);

	RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_EX_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウの作成
	hWnd = CreateWindow(
		CLASS_NAME,         //使用するウィンドウクラス
		WINDOW_CAPTION,     //ウィンドウの名前
		WS_OVERLAPPEDWINDOW,//
		CW_USEDEFAULT,      //x
		CW_USEDEFAULT,      //y
		rc.right - rc.left,  //幅
		rc.bottom - rc.top, //高さ
		NULL,               //ウィンドウの親
		NULL,               //ウィンドウメニューの設定
		hInstance,          //アプリケーション本体の変数
		NULL                //
	);

	if (!hWnd) {
		DWORD error = GetLastError();
		char errorMessage[256];
		sprintf_s(errorMessage, "CreateWindow failed with error: %lu", error);
		MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	//DirectXの初期化（ウィンドウを作成した後に行う）
	
	if (FAILED(FirstInit(hInstance, hWnd, true)))
	{
		return -1;
	}


	//時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	//フレームカウント初期化
	timeBeginPeriod(1);
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	//ウィンドウ表示(Init()の後に呼ばないとダメ)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//メッセージループ
	MSG    msg;
	SceneManager& sceneManager = SceneManager::GetInstance();


	
	sceneManager.RegisterScene(SCENE_TITLE, []() { return std::make_unique<TitleScene>(); });
	sceneManager.RegisterScene(SCENE_OP, []() { return std::make_unique<OP_Scene>(); });
	sceneManager.RegisterScene(SCENE_STAGE_SELECT, []() { return std::make_unique<StageSelectScene>(); });
	sceneManager.RegisterScene(SCENE_GAME, []() { return std::make_unique<GameScene>(); });
	sceneManager.RegisterScene(SCENE_RESULT, []() { return std::make_unique<ResulttScene>(); });
	

	//初期シーンの設定
	sceneManager.ChangeScene(SCENE_TITLE);
	

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
		
			
			

			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;

				wsprintf(g_DebugStr, WINDOW_CAPTION);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], "FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

			

				sceneManager.Update();

				sceneManager.Draw();

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);

	//終了
	game.Finalize();
	return (int)msg.wParam;
}

//プロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
	case WM_ACTIVATEAPP:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(uMsg, wParam, lParam);
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);//ウィンドウ消去
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);//アプリケーション終了
		return 0;
	};

	//その他のメッセージはwindowsに任せる
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



HRESULT FirstInit(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダリング処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	//サウンドの初期化
	InitSound(hWnd);

	//ポリゴン
	InitSprite();

	return S_OK;
}

void FinalFinalize()
{
	//ポリゴン
	UninitSprite();

	//サウンドの終了処理
	UninitSound();

	//レンダリングの終了処理
	UninitRenderer();
}


HWND* GetWindowsHandle()
{
	return &hWnd;
}

void SetWindowsHandle(HWND temp)
{
	hWnd = temp;
}


