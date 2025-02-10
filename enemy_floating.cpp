//-----------------------------------------------------------------------------------------------------
// #name enemy_floating.cpp
// #description ���V�G
// #make 2025/1/17�@���j�S
// #update 2025/1/17 
// #comment �ǉ��E�C���\��
// 
// 
//----------------------------------------------------------------------------------------------------
#include "enemy_floating.h"
#include "sprite.h"
#include "texture.h"
#include "collider_type.h"
#include "player_position.h"


static ID3D11ShaderResourceView* g_EnemyFloating_Texture = NULL;	//���I�G�l�~�[�̃e�N�X�`��

EnemyFloating::EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_FLOATING_LIFE, ENEMY_FLOATING_DAMAGE, ENEMY_FLOATING_SOULGAGE, ENEMY_FLOATING_SCORE, true, false)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;							//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�ture
	body.position.Set(position.x, position.y);			//�|�W�V�������Z�b�g
	body.angle = angle;									//�p�x�̒�`
	body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
	body.fixedRotation = true;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�
	body.gravityScale = 0.0f;


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
	fixture.isSensor = true;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* enemy_floating_fixture = GetBody()->CreateFixture(&fixture);//Body���Ƀt�B�N�X�`����o�^����

//�v���C���[�����m����Z���T�[
//--------------------------------------------
	b2PolygonShape sensor_shape;                         //shape�ɂ͐F�X�Ȍ^������@�T�[�N���Ƃ��������
	float one_square = 1.0f / BOX2D_SCALE_MANAGEMENT;	//�@1�}�X�̃T�C�Y
	sensor_shape.SetAsBox( size.x * 0.5f + (one_square * 7), size.y * 0.5f + (one_square * 7) );//�����蔻���o�^����4�_�@+((one_square * 7))f�͕������v14�}�X���������邽��

	b2FixtureDef sensor_fixture;
	sensor_fixture.shape = &sensor_shape;    //�V���[�v���t�B�N�X�`���ɓo�^����
	sensor_fixture.density = 1.0f;    //���x
	sensor_fixture.friction = 0.0f;  //���C
	sensor_fixture.restitution = 0.0f;//�����W��
	sensor_fixture.isSensor = true;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* enemy_sensor_fixture = GetBody()->CreateFixture(&sensor_fixture);//Body���Ƀt�B�N�X�`����o�^����


	// �J�X�^���f�[�^���쐬���Đݒ�
	// ���I�G�l�~�[�ɒl��o�^
	// ���I�G�l�~�[�Ƀ��[�U�[�f�[�^��o�^
	ObjectData* data = new ObjectData{ collider_enemy_floating };
	ObjectData* sensor_data = new ObjectData{ collider_enemy_floating_sensor };
	enemy_floating_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
	enemy_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(sensor_data);
	data->object_name = Object_Enemy_Floating;
	sensor_data->object_name = Object_Enemy_Floating;
	int ID = data->GenerateID();
	data->id = ID;
	sensor_data->id = ID;
	SetID(ID);
}

void EnemyFloating::Initialize()
{
	if (g_EnemyFloating_Texture == NULL)
	{
		g_EnemyFloating_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");//���I�G�l�~�[�̃e�N�X�`��
	}
}

void EnemyFloating::Finalize()
{

	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetBody() != nullptr)
	{
		world->DestroyBody(GetBody());
	}

	if (g_EnemyFloating_Texture != NULL)
	{
		UnInitTexture(g_EnemyFloating_Texture);
		g_EnemyFloating_Texture = NULL;
	}
}

void EnemyFloating::Update()
{
	if (GetUse() && m_sensed_player)
	{
		Move();
	}
	else if (!GetUse())
	{
		//�\�E���𗎂Ƃ�
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(GetBody()->GetPosition(), { 1.0f,2.0f }, 0.0f, GetSoulgage());

		//���[���h�ɓo�^����body�̍폜
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());

		//�I�u�W�F�N�g�}�l�[�W���[���̃G�l�~�[�폜
		ObjectManager& object_manager = ObjectManager::GetInstance();
		object_manager.DestroyEnemyFloating(GetID());
	}
}

void EnemyFloating::Draw()
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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ GetSize().x * scale , GetSize().y * scale }
	);
}

//�ړ�
void EnemyFloating::Move()
{
	//�v���C���[�Ǐ](�Ȉ�)
	//�v���C���[�̃|�W�V�����擾
	b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

	//  �v���C���[�֌������x�N�g��
	b2Vec2 vec;
	vec.x = player_position.x - GetBody()->GetPosition().x;
	vec.y = player_position.y - GetBody()->GetPosition().y;



	GetBody()->SetLinearVelocity(b2Vec2(vec.x * m_speed, vec.y * m_speed));

}
