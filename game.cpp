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
#include"box2d_world.h"
#include"square_collider.h"
#include"directx_controller.h"
#include"game.h"
#include"player.h"
#include"feild.h"


//���̊Ǘ����@�ǂ��Ȃ�H
SquareCollider* square_collider[100];

Player* player_collider;


DirectInputController controller;



HRESULT Game::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//�����_�����O�����̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	//�T�E���h�̏�����
	InitSound(hWnd);

	//�|���S��
	InitSprite();

	//�R���g���[���[�̏�����
	controller.Initialize(hInstance,hWnd);

	
	//�C���X�^���X���擾
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();

	//���[���h�𐶐�
	b2World* world = new b2World(b2Vec2(0.0f, 10.f));

	//�����������[���h�̃|�C���^���Z�b�g
	box2d_world.SetBox2dWorldPointer(world);


	//�e�L�g�[�ɐݒ肵��

	//�v���C���[
	player_collider->Init();

	//Init
	player_collider = new Player(XMFLOAT2(5.0f, 5.0f), XMFLOAT2(1.0f, 1.0f), 0.0f, false);



	Field::Init(30,20);

	





	//// �n�ڃW���C���g���쐬
	//b2WeldJointDef weldJointDef;
	//weldJointDef.bodyA =square_collider[2]->GetColliderBody();
	//weldJointDef.bodyB = square_collider[3]->GetColliderBody();
	//weldJointDef.localAnchorA.Set(1.0f, 0.0f); // �{�f�BA�̉E�[
	//weldJointDef.localAnchorB.Set(-1.0f, 0.0f); // �{�f�BB�̍��[
	//weldJointDef.collideConnected = false;  // �W���C���g�Őڑ����ꂽ���̓��m���Փ˂��Ȃ�

	//b2WeldJoint* weldJoint = (b2WeldJoint*)world->CreateJoint(&weldJointDef);


	return S_OK;
}

void Game::Uninit(void)
{
	//�|���S��
	UninitSprite();

	//�R���g���[���[�̏I������
	controller.Release();

	//�T�E���h�̏I������
	UninitSound();


	Field::Uninit();

	//�����_�����O�̏I������
	UninitRenderer();
}



void Game::Update(void)
{
	//�C���X�^���X���擾
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	//���[���h�̃X�e�b�v���Ă�
	box2d_world.GetBox2dWorldPointer()->Step(1.f / 60.f, 8, 3);



	


	controller.CheckInput();
}


void Game::Draw(void)
{
	//�o�b�t�@�N���A
	Clear();

	//2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);

	//�����ɕ`�揈��

	//�������牺�̓e�X�g
	
	player_collider->Draw();

	Field::Draw();
	


	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();
}

Game::Game()
{
}

Game::~Game()
{
}