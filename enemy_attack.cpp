//-----------------------------------------------------------------------------------------------------
// #name enemyAttack.cpp
// #description �G�l�~�[�̍U����cpp�t�@�C��
// #make 2024/12/20
// #update 2024/12/20
// #comment �ǉ��E�C���\��
//          �E�X�e�[�^�X����
//           
//----------------------------------------------------------------------------------------------------

#include"enemy_attack.h"
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

static ID3D11ShaderResourceView* g_EnemyAttack_Texture = NULL;	//���I�G�l�~�[�̃e�N�X�`��

EnemyAttack::EnemyAttack(b2Vec2 position, b2Vec2 body_size, float angle)
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

	b2Fixture* enemy_attack_fixture = GetBody()->CreateFixture(&fixture);//Body���Ƀt�B�N�X�`����o�^����

	// �J�X�^���f�[�^���쐬���Đݒ�
	// ���I�G�l�~�[�ɒl��o�^
	// ���I�G�l�~�[�Ƀ��[�U�[�f�[�^��o�^
	ObjectData* data = new ObjectData{ collider_enemy_attack };
	enemy_attack_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
	data->object_name = Object_Enemy_Attack;
	int ID = data->GenerateID();
	data->id = ID;
	SetID(ID);

	Initialize();
}

void EnemyAttack::Initialize()
{
	g_EnemyAttack_Texture = InitTexture(L"asset\\texture\\sample_texture\\xxx_enemy_attack.png");//�G�l�~�[�̃Z���T�[�̃e�N�X�`��
}

void EnemyAttack::Finalize()
{
	UnInitTexture(g_EnemyAttack_Texture);

	//�I�u�W�F�N�g�}�l�[�W���[���̃G�l�~�[�폜
	ObjectManager& object_manager = ObjectManager::GetInstance();
	object_manager.DestroyEnemyAttack(GetID());

	//���[���h�ɓo�^����body�̍폜
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	world->DestroyBody(GetBody());
	m_body = nullptr;
}

void EnemyAttack::Update()
{
	m_count++;
	if (m_count == m_frame)
	{
		//���[���h�ɓo�^����body�̍폜
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());

		//�I�u�W�F�N�g�}�l�[�W���[���̃G�l�~�[�폜
		ObjectManager& object_manager = ObjectManager::GetInstance();
		object_manager.DestroyEnemyDynamic(GetID());
	}
}

void EnemyAttack::Draw()
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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyAttack_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ GetSize().x * scale , GetSize().y * scale }
	);
}

//�U�����v���C���[�ɐG�ꂽ���̏���
void EnemyAttack::CollisionPlayer()
{
	//��x�U��������������_���[�W���������Ȃ�
	if (m_hit)
	{
		return;
	}

	//�_���[�W��^����
	PlayerStamina::EditPlayerStaminaValue(-GetDamage());
	//�Փˍ�
	m_hit = true;
}