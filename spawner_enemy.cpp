//-----------------------------------------------------------------------------------------------------
// #name spawner_enemy.cpp
// #description �X�|�i�[�̃\�[�X�t�@�C��
// #make 2025/02/21
// #update 2025/02/24
// #comment �ǉ��E�C���\��
//          �E
//           
//----------------------------------------------------------------------------------------------------

#include "spawner_enemy.h"
#include"include/box2d/box2d.h"
#include"enemy_dynamic.h"
#include"world_box2d.h"
#include"texture.h"
#include"sprite.h"
#include"contactlist.h"
#include<Windows.h>
#include"create_filter.h"
#include"object_manager.h"
#include"player_position.h"

static ID3D11ShaderResourceView* g_EnemySpawner_Texture;		  //���I�G�l�~�[�̃e�N�X�`��

SpawnerEnemy::SpawnerEnemy(b2Vec2 position, b2Vec2 size, float angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;							//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�ture
	body.position.Set(position.x, position.y);			//�|�W�V�������Z�b�g
	body.angle = angle;									//�p�x�̒�`
	body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
	body.fixedRotation = true;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
	b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���

	SetBody(world->CreateBody(&body));//Body�����[���h�ɌŒ�


	SetSize(size);//�\���p�ɃT�C�Y���Z�b�g���Ƃ��A�\���̂Ƃ���GetSize���Ăяo��

	size.x = size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
	size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	b2PolygonShape shape;                         //shape�ɂ͐F�X�Ȍ^������@�T�[�N���Ƃ��������
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//�����蔻���o�^����4�_�@*0.5����̂�

	b2FixtureDef fixture;
	fixture.shape = &shape;    //�V���[�v���t�B�N�X�`���ɓo�^����
	fixture.density = 0.1f;    //���x
	fixture.friction = 0.05f;  //���C
	fixture.restitution = 0.0f;//�����W��
	fixture.isSensor = true;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	//Body�Ƀt�B�N�X�`����o�^����
	b2Fixture* enemy_static_fixture = GetBody()->CreateFixture(&fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* data = new ObjectData{ collider_spawner_enemy };
	enemy_static_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

	//ID�o�^
	data->object_name = Object_Spawner_Enemy;
	int ID = data->GenerateID();
	data->id = ID;
	SetID(ID);
}

void SpawnerEnemy::Initialize()
{
	if(!g_EnemySpawner_Texture)
	{
		g_EnemySpawner_Texture = InitTexture(L"asset\\texture\\enemy_texture\\enemy_spawner.png");
	}
}

void SpawnerEnemy::Finalize()
{
	if (g_EnemySpawner_Texture)
	{
		UnInitTexture(g_EnemySpawner_Texture);
		g_EnemySpawner_Texture = NULL;
	}

	//���[���h�ɓo�^����body�̍폜
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	world->DestroyBody(GetBody());
	SetBody(nullptr);
}

void SpawnerEnemy::Update()
{
	if(m_in_screen)
	{
		m_spawn_counter++;
		m_spawn_counter %= m_spawn_space;

		if (m_spawn_counter == 0)
		{
			ObjectManager& object_manager = ObjectManager::GetInstance();
			b2Vec2 pos = GetBody()->GetPosition();
			object_manager.AddEnemyDynamic(pos, b2Vec2(1.0f, 1.0f), 0.0f);
		}
	}
}

void SpawnerEnemy::Draw()
{
	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 position = GetBody()->GetPosition();

	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

	//�\��e�N�X�`�����w��
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemySpawner_Texture);

	DrawSprite(
		{ draw_x,
		  draw_y },
		0,
		{ GetSize().x * scale,GetSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);
}