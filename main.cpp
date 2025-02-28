//-----------------------------------------------------------------------------------------------------
// #name main.cpp
// #description main 
// #make 2024/11/02縲縲豌ｸ驥守ｾｩ荵
// #update 2024/11/02
// #comment 霑ｽ蜉繝ｻ菫ｮ豁｣莠亥ｮ
//          繝ｻ蝓ｺ譛ｬ逧�↓謗域･ｭ騾壹ｊ縺ｮ繝�Φ繝励Ξ繝ｼ繝亥ｼ輔▲蠑ｵ縺｣縺ｦ縺阪◆
//          
//----------------------------------------------------------------------------------------------------

#include <Windows.h>
#include "renderer.h"
#include "sprite.h"
#include "keyboard.h"
#include "sound.h"
#include"game.h"
#include"scene.h"
#include"stage_select.h"
#include"sound.h"
#include"result.h"

#include"video_texture.h"



//繝ｩ繧､繝悶Λ繝ｪ縺ｮ繝ｪ繝ｳ繧ｯ
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//繝槭け繝ｭ螳夂ｾｩ
#define CLASS_NAME "GameWindow"
#define WINDOW_CAPTION	"このウィンドウを消すとプログラム終了"

//繝励Ο繝医ち繧､繝怜ｮ｣險
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



//繧ｰ繝ｭ繝ｼ繝舌Ν螟画焚
#ifdef _DEBUG
int g_CountFPS;
char g_DebugStr[2048] = WINDOW_CAPTION;
#endif//_DEBUG


//繝｡繧､繝ｳ
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);


	

	//繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ縺ｮ逋ｻ骭ｲ
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	RegisterClass(&wc);

	RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_EX_OVERLAPPEDWINDOW, FALSE);

	//繧ｦ繧｣繝ｳ繝峨え縺ｮ菴懈�
	HWND hWnd = CreateWindow(
		CLASS_NAME,         //菴ｿ逕ｨ縺吶ｋ繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ
		WINDOW_CAPTION,     //繧ｦ繧｣繝ｳ繝峨え縺ｮ蜷榊燕
		WS_OVERLAPPEDWINDOW,//
		CW_USEDEFAULT,      //x
		CW_USEDEFAULT,      //y
		rc.right - rc.left,  //蟷
		rc.bottom - rc.top, //鬮倥＆
		NULL,               //繧ｦ繧｣繝ｳ繝峨え縺ｮ隕ｪ
		NULL,               //繧ｦ繧｣繝ｳ繝峨え繝｡繝九Η繝ｼ縺ｮ險ｭ螳
		hInstance,          //繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ譛ｬ菴薙�螟画焚
		NULL                //
	);

	if (!hWnd) {
		DWORD error = GetLastError();
		char errorMessage[256];
		sprintf_s(errorMessage, "CreateWindow failed with error: %lu", error);
		MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	//DirectX縺ｮ蛻晄悄蛹厄ｼ医え繧｣繝ｳ繝峨え繧剃ｽ懈�縺励◆蠕後↓陦後≧�
	
	if (FAILED(FirstInit(hInstance, hWnd, true)))
	{
		return -1;
	}

	//Game& game = Game::GetInstance();


	//譎る俣險域ｸｬ逕ｨ
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	//繝輔Ξ繝ｼ繝繧ｫ繧ｦ繝ｳ繝亥�譛溷喧
	timeBeginPeriod(1);
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	//繧ｦ繧｣繝ｳ繝峨え陦ｨ遉ｺ(Init()縺ｮ蠕後↓蜻ｼ縺ｰ縺ｪ縺�→繝繝｡)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//繝｡繝�そ繝ｼ繧ｸ繝ｫ繝ｼ繝
	MSG    msg;
	SceneManager& sceneManager = SceneManager::GetInstance();


	
	sceneManager.RegisterScene(SCENE_TITLE, []() { return std::make_unique<TitleScene>(); });
	sceneManager.RegisterScene(SCENE_OP, []() { return std::make_unique<VideoScene>(); });
	sceneManager.RegisterScene(SCENE_STAGE_SELECT, []() { return std::make_unique<StageSelectScene>(); });
	sceneManager.RegisterScene(SCENE_GAME, []() { return std::make_unique<GameScene>(); });
	sceneManager.RegisterScene(SCENE_RESULT, []() { return std::make_unique<ResultScene>(); });
	

	//蛻晄悄繧ｷ繝ｼ繝ｳ縺ｮ險ｭ螳

	//髢狗匱荳ｭ縺ｯ繧ｲ繝ｼ繝繧ｷ繝ｼ繝ｳ縺九ｉ繧ｹ繧ｿ繝ｼ繝
	#ifdef _DEBUG
	sceneManager.SetStageName(STAGE_SELECT);

	sceneManager.ChangeScene(SCENE_TITLE);

	#else
	sceneManager.ChangeScene(SCENE_TITLE);
  sceneManager.SetStageName(STAGE_SELECT);
	#endif
	

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

	//邨ゆｺ
	FinalFinalize();
	return (int)msg.wParam;
}

//繝励Ο繧ｷ繝ｼ繧ｸ繝｣
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
		if (MessageBox(hWnd, "本当に終了してよろしいですか？","確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);//ウィンドウ消去
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);//繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ邨ゆｺ
		return 0;
	};

	//縺昴�莉悶�繝｡繝�そ繝ｼ繧ｸ縺ｯwindows縺ｫ莉ｻ縺帙ｋ
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



HRESULT FirstInit(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ蜃ｦ逅��蛻晄悄蛹
	InitRenderer(hInstance, hWnd, bWindow);

	VideoTexture::createAPI();

	//繧ｵ繧ｦ繝ｳ繝峨�蛻晄悄蛹
	CRIInitialize();

	//繝昴Μ繧ｴ繝ｳ
	InitSprite();

	return S_OK;
}

void FinalFinalize()
{
	//繝昴Μ繧ｴ繝ｳ
	UninitSprite();

	//繧ｵ繧ｦ繝ｳ繝峨�邨ゆｺ��逅
	CRIFinalize();

	VideoTexture::destroyAPI();

	//繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ縺ｮ邨ゆｺ��逅
	UninitRenderer();
}


