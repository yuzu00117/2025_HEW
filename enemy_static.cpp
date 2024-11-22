//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description �ÓI�G�l�~�[��cpp�t�@�C��
// #make 2024/11/19
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_static.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"

//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_enemy_static_texture = NULL;

void EnemyStatic::Initialize()
{
	//�e�N�X�`���ǂݍ���
	g_enemy_static_texture= InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
}

void EnemyStatic::Update()
{
	
}

void EnemyStatic::Draw()
{
	//�w��field�Ɠ��l

	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 position;
	position.x = GetEnemyBody()->GetPosition().x;
	position.y = GetEnemyBody()->GetPosition().y;

	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



	//�\��e�N�X�`�����w��
	GetDeviceContext()->PSSetShaderResources(1, 1, &g_enemy_static_texture);


	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetEnemyBody()->GetAngle(),
		{ GetEnemySize().x * scale , GetEnemySize().y * scale }
	);
}

void EnemyStatic::Finalize()
{
	//�w��field�Ɠ��l

	if (GetEnemyBody()) {
		// �{�f�B���폜
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetEnemyBody());
		SetEnemyBody(nullptr);
	}
	if (g_enemy_static_texture)
	{
		UnInitTexture(g_enemy_static_texture);
	}
}