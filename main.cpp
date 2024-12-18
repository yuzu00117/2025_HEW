//-----------------------------------------------------------------------------------------------------
// #name main.cpp
// #description main 
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
//          �E��{�I�Ɏ��ƒʂ�̃e���v���[�g���������Ă���
//          
//----------------------------------------------------------------------------------------------------

#include <Windows.h>
#include "renderer.h"
#include "sprite.h"
#include "keyboard.h"
#include "sound.h"
#include"game.h"
#include"scene.h"



//���C�u�����̃����N
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//�}�N����`
#define CLASS_NAME "GameWindow"
#define WINDOW_CAPTION "���̃E�B���h�E�������ƃv���O�����I��"

//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



//�O���[�o���ϐ�
HWND hWnd;

#ifdef _DEBUG
int g_CountFPS;
char g_DebugStr[2048] = WINDOW_CAPTION;
#endif//_DEBUG


//���C��
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);




	Game& game = Game::GetInstance();

	

	//�E�B���h�E�N���X�̓o�^
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	RegisterClass(&wc);

	RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_EX_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E�̍쐬
	hWnd = CreateWindow(
		CLASS_NAME,         //�g�p����E�B���h�E�N���X
		WINDOW_CAPTION,     //�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,//
		CW_USEDEFAULT,      //x
		CW_USEDEFAULT,      //y
		rc.right - rc.left,  //��
		rc.bottom - rc.top, //����
		NULL,               //�E�B���h�E�̐e
		NULL,               //�E�B���h�E���j���[�̐ݒ�
		hInstance,          //�A�v���P�[�V�����{�̂̕ϐ�
		NULL                //
	);

	if (!hWnd) {
		DWORD error = GetLastError();
		char errorMessage[256];
		sprintf_s(errorMessage, "CreateWindow failed with error: %lu", error);
		MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	//DirectX�̏������i�E�B���h�E���쐬������ɍs���j
	
	if (FAILED(FirstInit(hInstance, hWnd, true)))
	{
		return -1;
	}


	//���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	//�t���[���J�E���g������
	timeBeginPeriod(1);
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	//�E�B���h�E�\��(Init()�̌�ɌĂ΂Ȃ��ƃ_��)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	MSG    msg;
	SceneManager& sceneManager = SceneManager::GetInstance();


	
	sceneManager.RegisterScene(SCENE_TITLE, []() { return std::make_unique<TitleScene>(); });
	sceneManager.RegisterScene(SCENE_OP, []() { return std::make_unique<OP_Scene>(); });
	sceneManager.RegisterScene(SCENE_STAGE_SELECT, []() { return std::make_unique<StageSelectScene>(); });
	sceneManager.RegisterScene(SCENE_GAME, []() { return std::make_unique<GameScene>(); });
	sceneManager.RegisterScene(SCENE_RESULT, []() { return std::make_unique<ResulttScene>(); });
	

	//�����V�[���̐ݒ�
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

	//�I��
	game.Finalize();
	return (int)msg.wParam;
}

//�v���V�[�W��
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
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);//�E�B���h�E����
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);//�A�v���P�[�V�����I��
		return 0;
	};

	//���̑��̃��b�Z�[�W��windows�ɔC����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



HRESULT FirstInit(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//�����_�����O�����̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	//�T�E���h�̏�����
	InitSound(hWnd);

	//�|���S��
	InitSprite();

	return S_OK;
}

void FinalFinalize()
{
	//�|���S��
	UninitSprite();

	//�T�E���h�̏I������
	UninitSound();

	//�����_�����O�̏I������
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


