#include "main.h"
#include "scene.h"
#include "title.h"
#include "gameselect.h"
#include "game.h"
#include "gameover.h"
#include "result.h"
#include "renderer.h"
#include "sound.h"
#include "directx_controller.h"
#include "word.h"
#include "debug.h"


#include "sprite.h"

//�V�[���̐؂�ւ��̃X�C�b�`
static int g_Scene;

//�R���g���[��
DirectInputController controller;

//�����������������Ƃł�
HRESULT AllInitializeScene(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

    //�����_�����O�����̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	//�|���S��
	InitSprite();

	//�����i�G�j
	InitializeWord();

	//�R���g���[���[�̏�����
	controller.Initialize(hInstance, hWnd);


#ifndef _DEBUG
	InitializeDebug();
#endif //_DEBUG

	return S_OK;

}

//����������
void InitializeScene()
{
	g_Scene = SCENE_NONE;

	ChangeScene(SCENE_TITLE);
}


//�I������
void FinalizeScene()
{

	Game& game = Game::GetInstance();



	//�S����Uninit�܂���finalize���I�����������邩



	//�|���S���I������
	UninitSprite();

	//�R���g���[���[�̏I������
	controller.Release();



	game.Finalize();//�Ō�ɌĂяo���t�@�C��

#ifndef _DEBUG
	FinalizeDebug();
#endif //_DEBUG

}

//�X�V����
void UpdateScene()
{

	Game& game = Game::GetInstance();
	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAMESELECT:
		UpdateGameSelect();
		break;
	case SCENE_GAME:
		game.Update();
		break;
	case SCENE_GAMEOVER:
		UpdateGameOver();
		break;
	case SCENE_RESULT:
		UpdateResult();
		break;
	default:
		break;

	}

	controller.CheckInput();
}

//�`�揈��
void DrawScene()
{
	Game& game = Game::GetInstance();
	Clear();

	SetDepthEnable(false);

	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAMESELECT:
		DrawGameSelect();
		break;
	case SCENE_GAME:
		game.Draw();
		break;
	case SCENE_GAMEOVER:
		DrawGameOver();
		break;
	case SCENE_RESULT:
		DrawResult();
		break;
	default:
		break;

	}

	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();

}





void ChangeScene(GAME_SCENE scene)
{
	Game& game = Game::GetInstance();
	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		FinalizeTitle();
		break;
	case SCENE_GAMESELECT:
		FinalizeGameSelect();
		break;
	case SCENE_GAME:
		//�I�u�W�F�N�g�������ꂽ��ǉ�������j
		break;
	case SCENE_GAMEOVER:
		FinalizeGameOver();
		break;
	case SCENE_RESULT:
		FinalizeResult();
		break;
	default:
		break;

	}

	g_Scene = scene;
	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		InitializeTitle();
		break;
	case SCENE_GAMESELECT:
		InitializeGameSelect();
		break;
	case SCENE_GAME:
		game.Initialize();
		break;
	case SCENE_GAMEOVER:
		InitializeGameOver();
		break;
	case SCENE_RESULT:
		InitializeResult();
		break;
	default:
		break;

	}
}