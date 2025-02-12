//-----------------------------------------------------------------------------------------------------
// #name one-way_platform.cpp
// #description ����̃I�u�W�F�N�g�̃w�b�_�[
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//			�E����Ȃ�E�����ɓ|�ꂽ���Ƃ����������邩���ˁ[
//----------------------------------------------------------------------------------------------------
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"one-way_platform.h"
#include"create_filter.h"



//�e�N�X�`���̓��ꕨ
//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_one_way_platform_Texture = NULL;//����u���b�N�̃e�N�X�`��
static ID3D11ShaderResourceView* g_one_way_platform1_Texture = NULL;//����u���b�N�e�N�X�`��
static ID3D11ShaderResourceView* g_ground_Texture = NULL;//����u���b�N�e�N�X�`���̂����ɕ\������w�i�̃e�N�X�`��
one_way_platform::one_way_platform(b2Vec2 Postion,b2Vec2 local_Postion, b2Vec2 size,bool object_contact)
{
	SetSize(size);

	// ���[���h�̃C���X�^���X���擾
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	SetlocalPosition(local_Postion);

	// �T�C�Y��ݒ�
	b2Vec2 one_way_platform_size;
	one_way_platform_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	one_way_platform_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	// ���[�J���|�W�V�����̃X�P�[���␳
	local_Postion.x /= BOX2D_SCALE_MANAGEMENT;
	local_Postion.y /= BOX2D_SCALE_MANAGEMENT;

	// ����ʍs�v���b�g�t�H�[���̃{�f�B���쐬
	b2BodyDef one_way_platform_body;
	one_way_platform_body.type = b2_staticBody;
	one_way_platform_body.position.Set(Postion.x + local_Postion.x, Postion.y + local_Postion.y);
	one_way_platform_body.fixedRotation = false;

	b2Body* m_one_way_platform_body = world->CreateBody(&one_way_platform_body);
	SetObject_one_way_platform_Body(m_one_way_platform_body);

	// �`����`
	b2PolygonShape one_way_platform_shape;
	one_way_platform_shape.SetAsBox(one_way_platform_size.x * 0.5, one_way_platform_size.y * 0.5);

	// �t�B�N�X�`���̍쐬�i�ʏ�̒n�ʁj
	b2FixtureDef one_way_platform_fixture;
	one_way_platform_fixture.shape = &one_way_platform_shape;
	one_way_platform_fixture.density = 3.0f;
	one_way_platform_fixture.friction = 0.5f;
	one_way_platform_fixture.restitution = 0.0f;
	one_way_platform_fixture.isSensor = false;
	one_way_platform_fixture.filter = createFilterExclude("one-way_platform_filter", { "object_filter","one-way_platform_filter" });

	b2Fixture* object_one_way_platform_fixture = m_one_way_platform_body->CreateFixture(&one_way_platform_fixture);
	SetChangeFixture(object_one_way_platform_fixture);

	// �J�X�^���f�[�^�̍쐬
	ObjectData* object_one_way_platform_data = new ObjectData{ collider_ground };
	object_one_way_platform_data->object_name = Object_one_way_platform;

	int ID = object_one_way_platform_data->GenerateID();
	object_one_way_platform_data->id = ID;
	SetID(ID);

	// ���[�U�[�f�[�^��ݒ�
	object_one_way_platform_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_one_way_platform_data);

	if (object_contact == true)
	{
		// �t�B�N�X�`����ǉ��i�I�u�W�F�N�g�Ƃ̏Փ˗p�j
		b2FixtureDef one_way_platform_object_fixture;
		one_way_platform_object_fixture.shape = &one_way_platform_shape;
		one_way_platform_object_fixture.density = 3.0f;
		one_way_platform_object_fixture.friction = 0.5f;
		one_way_platform_object_fixture.restitution = 0.0f;
		one_way_platform_object_fixture.isSensor = false;
		one_way_platform_object_fixture.filter = createFilterExclude("one-way_platform_filter", { "Player_filter","one-way_platform_filter" });

		b2Fixture* contact_object_one_way_platform_fixture = m_one_way_platform_body->CreateFixture(&one_way_platform_object_fixture);

		// �����ŐV���� `ObjectData` ����炸�A�����̃f�[�^���g��
		contact_object_one_way_platform_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_one_way_platform_data);
	}

};

one_way_platform::~one_way_platform()
{
}


void one_way_platform::Initialize()
{
	//���o�I�ɍ��Z���T�[�Ȃ̂���`���Ă���
	//�e�N�X�`���̃C���N���[�h

	if (g_one_way_platform1_Texture == NULL) {
		g_one_way_platform_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_one_way_platform.png");//�Z���T�[���I�t�@�����蔻�肠��
		g_one_way_platform1_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_one_way_platform1.png");//�Z���T�[���I���@�����蔻�薳��
		g_ground_Texture= InitTexture(L"asset\\texture\\stage_block\\1-1_block_soil_02.png");//�O���E���h�̃e�N�X�`��
	}
}

void one_way_platform::Update()
{
	//�v���C���[��荂�������ɂ���ƃZ���T�[���I�t��
	//�I�u�W�F�N�g�Ɋւ��Ă��ׂĊђ�
	float offset = 1.0f / BOX2D_SCALE_MANAGEMENT;
	bool shouldBeSensor = PlayerPosition::GetPlayerPosition().y + offset >= GetObject_one_way_platform_Body()->GetPosition().y;

	if (GetChangeFixture()->IsSensor() != shouldBeSensor) // ��Ԃ��ς��Ƃ����� SetSensor() ���Ă�
	{
		GetChangeFixture()->SetSensor(shouldBeSensor);
	}
}


void one_way_platform::Draw()
{
	///�������璲�����Ă�
	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;


	b2Vec2  one_way_platform_pos = GetObject_one_way_platform_Body()->GetPosition();

	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((one_way_platform_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((one_way_platform_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	//�w�i���ɕ`��
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_ground_Texture);
	DrawSprite(
		{ draw_x+GetlocalPosition().x*scale,
		  draw_y+(GetlocalPosition().y + GetSize().x) * scale},
		GetObject_one_way_platform_Body()->GetAngle(),
		{ GetSize().x * scale,GetSize().x * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);


	if (GetObject_one_way_platform_Body()->GetFixtureList()->IsSensor() == false)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_one_way_platform_Texture);
	}
	else
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_one_way_platform1_Texture);
	}
	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObject_one_way_platform_Body()->GetAngle(),
		{ GetSize().x * scale,GetSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);
}

void one_way_platform::Finalize()
{
	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetObject_one_way_platform_Body() != nullptr)
	{
		//body���폜����
		world->DestroyBody(one_way_platform_body);
	}

	if (g_one_way_platform_Texture != NULL)
	{
		//�e�N�X�`���̉��
		UnInitTexture(g_one_way_platform_Texture);
		UnInitTexture(g_one_way_platform1_Texture);

		g_one_way_platform1_Texture = NULL;
		g_one_way_platform_Texture = NULL;
	}


}