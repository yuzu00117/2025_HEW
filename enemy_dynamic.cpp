//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description ���I�G�l�~�[(�v���C���[�Ǐ])��cpp�t�@�C��
// #make 2024/11/20
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�X�e�[�^�X����
//			�E�K�v�ɉ������ړ����@�m�ύX(�n���o�[�W�����m�쐬�Ȃ�)
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_dynamic.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"
#include"player_stamina.h"
#include"contactlist.h"
#include"anchor_spirit.h"

EnemyDynamic* g_p_enemies_dynamic[ENEMY_MAX] = { nullptr };

EnemyDynamic::EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture)
	:Enemy(ENEMY_DYNAMIC_LIFE, ENEMY_DYNAMIC_DAMAGE, ENEMY_DYNAMIC_SOULGAGE, ENEMY_DYNAMIC_SCORE, true)
{
	//�e�N�X�`�����Z�b�g
	SetFieldTexture(texture);


	b2BodyDef body;
	body.type = b2_dynamicBody;							//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�ture
	body.position.Set(position.x, position.y);			//�|�W�V�������Z�b�g
	body.angle = angle;									//�p�x�̒�`
	body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
	body.fixedRotation = true;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
	b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���
	SetFieldBody(world->CreateBody(&body));				//Body�����[���h�ɌŒ�


	SetSize(body_size);//�\���p�ɃT�C�Y���Z�b�g���Ƃ��A�\���̂Ƃ���GetSize���Ăяo��



	b2Vec2 size;
	size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
	size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape;                         //shape�ɂ͐F�X�Ȍ^������@�T�[�N���Ƃ��������
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//�����蔻���o�^����4�_�@*0.5����̂�

	b2FixtureDef fixture;
	fixture.shape = &shape;    //�V���[�v���t�B�N�X�`���ɓo�^����
	fixture.density = 1.0f;    //���x
	fixture.friction = 0.05f;  //���C
	fixture.restitution = 0.0f;//�����W��
	fixture.isSensor = false;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* enemy_dynamic_fixture = GetFieldBody()->CreateFixture(&fixture);//Body�Ƀt�B�N�X�`����o�^����

	// �J�X�^���f�[�^���쐬���Đݒ�
	// ���I�G�l�~�[�ɒl��o�^
	// ���I�G�l�~�[�Ƀ��[�U�[�f�[�^��o�^
	ObjectData* data = new ObjectData{ collider_enemy_dynamic };
	enemy_dynamic_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (!g_p_enemies_dynamic[i])
		{
			g_p_enemies_dynamic[i] = this;
			return;
		}
	}
}

void EnemyDynamic::Update()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_dynamic[i])
		{
			if(g_p_enemies_dynamic[i]->GetUse())
			{
				g_p_enemies_dynamic[i]->UpdateEnemy();
			}
			else
			{
				//���[���h�ɓo�^����body�̍폜(�ǉ��\��)
				Box2dWorld& box2d_world = Box2dWorld::GetInstance();
				b2World* world = box2d_world.GetBox2dWorldPointer();
				world->DestroyBody(g_p_enemies_dynamic[i]->GetFieldBody());

				Field::DeleteFieldObject(g_p_enemies_dynamic[i]->GetFieldBody());
				g_p_enemies_dynamic[i] = nullptr;
			}
		}
	}
}

void EnemyDynamic::UpdateEnemy()
{
	//�v���C���[�Ǐ](�Ȉ�)
	//�v���C���[�̃|�W�V�����擾
	b2Vec2 player_position;
	player_position.x = PlayerPosition::GetPlayerPosition().x;
	player_position.y = PlayerPosition::GetPlayerPosition().y;

	//�ړ�����
	b2Vec2 enemy_vector;
	enemy_vector.x = player_position.x - GetFieldBody()->GetPosition().x;
	enemy_vector.y = player_position.y - GetFieldBody()->GetPosition().y;

	//�ړ���
	b2Vec2 enemy_move;
	enemy_move.x = (enemy_vector.x * m_speed);
	enemy_move.y = (enemy_vector.y * m_speed);

	GetFieldBody()->ApplyForceToCenter(b2Vec2(enemy_move.x, enemy_move.y), true);
}

void EnemyDynamic::CollisionPlayer(b2Body* collision_enemy)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_dynamic[i])
		{
			if (g_p_enemies_dynamic[i]->GetFieldBody() == collision_enemy)
			{
				PlayerStamina::EditPlayerStaminaValue(-g_p_enemies_dynamic[i]->GetDamage());
				g_p_enemies_dynamic[i]->SetUse(false);
				return;
			}
		}
	}
}

void EnemyDynamic::CollisionPulledObject(b2Body* collision_enemy)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_dynamic[i])
		{
			if (g_p_enemies_dynamic[i]->GetFieldBody() == collision_enemy)
			{
				AnchorSpirit::EditAnchorSpiritValue(-g_p_enemies_dynamic[i]->GetDamage());
				g_p_enemies_dynamic[i]->SetUse(false);
				return;
			}
		}
	}
}

void EnemyDynamic::Finalize()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_p_enemies_dynamic[i])
		{
			g_p_enemies_dynamic[i] = nullptr;
		}
	}
}