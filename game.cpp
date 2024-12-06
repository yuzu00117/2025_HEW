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
#include"word.h"
#include"debug.h"
#include"scene.h"


void Game::Initialize()
{
	//�v���C���[�̏�����
	player.Initialize();

	//�A���J�[�̏�����
	Anchor::Initialize();

	//�t�B�[���h�̏�����
	Field::Initialize(90, 20);

	//�\�E���Q�[�WUI�̏�����
	stamina_spirit_gauge.Initialize();

	b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
	// �Փ˃��X�i�[�����[���h�ɓo�^
	MyContactListener& contactListener = MyContactListener::GetInstance();
	world->SetContactListener(&contactListener);
}

void Game::Finalize(void)
{

	//�v���C���[�̏I������
	player.Finalize();

	//�A���J�[�I������
	Anchor::Finalize();

	//�t�B�[���h�̏I������
	Field::Finalize();

	//�����i�G�j
	FinalizeWord();

	//�\�E���Q�[�WUI�̏I������
	stamina_spirit_gauge.Finalize();

	//�����_�����O�̏I������
	UninitRenderer();

#ifdef _DEBUG
	//�f�o�b�O����
	FinalizeDebug();
#endif // _DEBUG
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


#ifdef _DEBUG
	//�f�o�b�O����
	UpdateDebug();
#endif // _DEBUG
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

	//�\�E���Q�[�WUI�̕`�揈��
	stamina_spirit_gauge.Draw();


#ifdef _DEBUG
	//�f�o�b�O����
	DrawDebug();
#endif // _DEBUG

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