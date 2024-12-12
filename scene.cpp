#include "main.h"
#include "scene.h"
#include "title.h"
#include "gameselect.h"
#include "game.h"
#include "gameover.h"
#include "result.h"
#include "renderer.h"
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
	//�Q�[����ʂ̏�����
	g_Scene = SCENE_NONE;

	//�ŏ��ɕ\������ꏊ
	ChangeScene(SCENE_TITLE);
}


//�I������
void FinalizeScene()
{
	//�Q�[������C���X�^���X���Ƃ��ăt�@�C�i���C�Y�ɌĂ�ł���
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
	case SCENE_TITLE://�^�C�g��
		UpdateTitle();
		break;
	case SCENE_GAMESELECT://�X�e�[�W�Z���N�g
		UpdateGameSelect();
		break;
	case SCENE_GAME://�Q�[��
		game.Update();
		break;
	case SCENE_GAMEOVER://�Q�[���I�[�o�[
		UpdateGameOver();
		break;
	case SCENE_RESULT://���U���g
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
	case SCENE_TITLE://�^�C�g��
		DrawTitle();
		break;
	case SCENE_GAMESELECT://�X�e�[�W�Z���N�g
		DrawGameSelect();
		break;
	case SCENE_GAME://�Q�[��
		game.Draw();
		break;
	case SCENE_GAMEOVER://�Q�[���I�[�o�[
		DrawGameOver();
		break;
	case SCENE_RESULT://���U���g
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
	case SCENE_TITLE://�^�C�g��
		FinalizeTitle();
		break;
	case SCENE_GAMESELECT://�Q�[���Z���N�g
		FinalizeGameSelect();
		break;
	case SCENE_GAME://�Q�[��
		//�I�u�W�F�N�g�������ꂽ��ǉ�������j
		break;
	case SCENE_GAMEOVER://�Q�[���I�[�o�[
		FinalizeGameOver();
		break;
	case SCENE_RESULT://���U���g
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
	case SCENE_TITLE://�^�C�g��
		InitializeTitle();
		break;
	case SCENE_GAMESELECT://�Q�[���Z���N�g
		InitializeGameSelect();
		break;
	case SCENE_GAME://�Q�[��
		game.Initialize();
		break;
	case SCENE_GAMEOVER://�Q�[���I�[�o�[
		InitializeGameOver();
		break;
	case SCENE_RESULT://���U���g
		InitializeResult();
		break;
	default:
		break;

	}
}