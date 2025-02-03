//-----------------------------------------------------------------------------------------------------
// #name static_to_dynamic_block.cpp
// #description ��̃I�u�W�F�N�g��CPP
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//        
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"player_position.h"
#include"static_to_dynamic_block.h"
#include"create_filter.h"
#include"collider_type.h"


//�e�N�X�`���̓��ꕨ
//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_BoxRock_Texture = NULL;//�l�p�̃R���C�_�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_coconut_Texture = NULL;//�ۂ̃R���C�_�[�̃e�N�X�`��





static_to_dynamic_block::static_to_dynamic_block(b2Vec2 Position, b2Vec2 size, collider_type_Box_or_Circle collider_type, int need_anchor_level)
{

	//�T�C�Y���Z�b�g
	SetSize(size);
	//�R���C�_�[���Z�b�g
	SetBox_or_Circle(collider_type);
	//�t���O���Z�b�g
	Set_Change_Dynamic_flag(false);




	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//�T�C�Y�̕␳������
	b2Vec2 object_size;
	object_size.x = size.x / BOX2D_SCALE_MANAGEMENT*0.95;
	object_size.y = size.y / BOX2D_SCALE_MANAGEMENT*0.95;

	b2BodyDef body;

	body.type = b2_staticBody;
	body.position.Set(Position.x, Position.y);
	body.fixedRotation = false;//��]����

	b2Body* m_body = world->CreateBody(&body);

	

	b2FixtureDef fixture;//�t�B�N�X�`�����쐬

	// �N���X���� b2Shape �������o�[�Ƃ��ĕێ�����ꍇ�̗�
	b2PolygonShape shape; // �N���X�̃����o�[�ϐ��Ƃ��ĕێ�
	b2CircleShape circleShape;

	if (collider_type == Box_collider)
	{
		shape.SetAsBox(object_size.x * 0.5, object_size.y * 0.5);
		fixture.shape = &shape; // �����o�[�ϐ����g�p
	}
	else if (collider_type == Circle_collider)
	{
		circleShape.m_radius = object_size.x * 0.5;
		fixture.shape = &circleShape; // �����o�[�ϐ����g�p
	}

	fixture.density = 3.0f;//���x
	fixture.friction = 0.3f;//���C
	fixture.restitution = 0.0f;//�����W��
	fixture.isSensor = false;//�Z���T�[���ǂ���
	fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* m_fixture = m_body->CreateFixture(&fixture);//�t�B�N�X�`�����{�f�B�ɓo�^

	SetObjectBody(m_body);//�{�f�B��o�^����

	ObjectData* object_data = new ObjectData{ collider_object };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);
	object_data->need_anchor_level = need_anchor_level;
	////--------------------------------------------------------------------------------------------------
	//�A���J�[�|�C���g�̃t�B�N�X�`��
	b2FixtureDef fixture_anchorpoint;

	// �N���X���� b2Shape �������o�[�Ƃ��ĕێ�����ꍇ�̗�
	b2PolygonShape shape_anchorpoint; // �N���X�̃����o�[�ϐ��Ƃ��ĕێ�
	b2CircleShape circleShape_anchorpoint;

	if (collider_type == Box_collider)
	{
		shape_anchorpoint.SetAsBox(object_size.x * 0.5*0.5, object_size.y * 0.5*0.5);
		fixture_anchorpoint.shape = &shape_anchorpoint; // �����o�[�ϐ����g�p
	}
	else if (collider_type == Circle_collider)
	{
		circleShape_anchorpoint.m_radius = object_size.x * 0.5*0.5;
		fixture_anchorpoint.shape = &circleShape_anchorpoint; // �����o�[�ϐ����g�p
	}

	fixture_anchorpoint.density = 3.0f;//���x
	fixture_anchorpoint.friction = 0.5f;//���C
	fixture_anchorpoint.restitution = 0.0f;//�����W��
	fixture_anchorpoint.isSensor = false;//�Z���T�[���ǂ���
	fixture_anchorpoint.filter = createFilterExclude("object_filter", {});

	b2Fixture* m_anchorpoint_fixture = m_body->CreateFixture(&fixture_anchorpoint);


	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	m_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);


	int ID = object_anchorpoint_data->GenerateID();
	object_anchorpoint_data->id = ID;
	SetID(ID);

	object_anchorpoint_data->object_name = Object_Static_to_Dynamic;

	object_anchorpoint_data->need_anchor_level = need_anchor_level;


};

static_to_dynamic_block::~static_to_dynamic_block()
{

}


void static_to_dynamic_block::Initialize()
{
	if (g_coconut_Texture == NULL) {
		g_coconut_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_coconut.png");
		g_BoxRock_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_fall_rock.png");
	}

}

void static_to_dynamic_block::Update()
{
	//flag���I���Ȃ瓮�I��
	if (Get_Change_Dynamic_flag() == true)
	{
		b2Body* body = GetObjectBody();
		body->SetType(b2_dynamicBody);//���I�ɕύX
	}

}
void static_to_dynamic_block::Change_dynamic()
{
	Set_Change_Dynamic_flag(true);
	SetIfPulling(true);
}


void static_to_dynamic_block::Draw()
{

	///�������璲�����Ă�




	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 RockPos = GetObjectBody()->GetPosition();




	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((RockPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((RockPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

	if (GetBox_or_Circle() == Box_collider)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_BoxRock_Texture);
	}
	else
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_coconut_Texture);
	}

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectBody()->GetAngle(),
		{ GetSize().x * scale,GetSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);




}

void static_to_dynamic_block::Finalize()
{
	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetObjectBody() != nullptr)
	{
		//�{�f�B�̍폜
		world->DestroyBody(object_body);
	}

	

	//�e�N�X�`���̉��
	UnInitTexture(g_BoxRock_Texture);
	UnInitTexture(g_coconut_Texture);

}

