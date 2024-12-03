//-----------------------------------------------------------------------------------------------------
// #name game.cpp
// #description game
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
//          �E�����ŏ������A�X�V�A�`��A�I���������Ǘ����Ă���
// �@�@�@�@ �E�����ɒ��Œl�Ԃ�����Ńe�X�g���Ă邩��A�e�X�g�I������������I
//          
//----------------------------------------------------------------------------------------------------

#include <Windows.h>
#include "renderer.h"
#include "sprite.h"
#include "keyboard.h"
#include "sound.h"
#include"include/box2d/box2d.h"
#include"directx_controller.h"
#include"game.h"
#include"contactlist.h"
#include"anchor.h"




HRESULT Game::Initialize(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//�����_�����O�����̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	//�T�E���h�̏�����
	InitSound(hWnd);

	//�|���S��
	InitSprite();

	//�R���g���[���[�̏�����
	controller.Initialize(hInstance,hWnd);

	//�v���C���[�̏�����
	player.Initialize();

	//�A���J�[�̏�����
	Anchor::Initialize();

	//�t�B�[���h�̏�����
	Field::Initialize();

	b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
	// �Փ˃��X�i�[�����[���h�ɓo�^
	MyContactListener& contactListener = MyContactListener::GetInstance();
	world->SetContactListener(&contactListener);

	return S_OK;
}

void Game::Finalize(void)
{
	//�|���S��
	UninitSprite();

	//�R���g���[���[�̏I������
	controller.Release();

	//�T�E���h�̏I������
	UninitSound();


	//�v���C���[�̏I������
	player.Finalize();

	//�A���J�[�I������
	Anchor::Finalize();

	//�t�B�[���h�̏I������
	Field::Finalize();

	
	//�����_�����O�̏I������
	UninitRenderer();
}



void Game::Update(void)
{

	// Box2D ���[���h�̃X�e�b�v�X�V
	b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
	world->Step(1.0f / 60.0f, 6, 2);

	//�v���C���[�̍X�V����
	player.Update();

	//�A���J�[�̍X�V����
	Anchor::Update();

	//�t�B�[���h�̍X�V����
	Field::Update();

	controller.CheckInput();
}


void Game::Draw(void)
{
	//�o�b�t�@�N���A
	Clear();

	//2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);

	//�v���C���[�̕`�揈��
	player.Draw();

	//�A���J�[�̕`�揈��
	Anchor::Draw();

	//�t�B�[���h�̕`�揈��
	Field::Draw();



	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();
}



/**
 * @fn  Game�N���X�̃R���X�g���N�^
 * @brief Game�N���X�̃����o�ϐ��̒��ɂ���N���X�ɃV���O���g���ŃC���X�^���X�������
 */
Game::Game()
{
	//�v���C���[�̃C���X�^���X�������ė���Game�N���X�̃����o��o�^����
	player = Player::GetInstance();//�V���O���g��

	
}

Game::~Game()
{
}