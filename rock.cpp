//-----------------------------------------------------------------------------------------------------
// #name rock.cpp
// #description ��̃I�u�W�F�N�g��CPP
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//			�E����Ȃ�E�����ɓ|�ꂽ���Ƃ����������邩���ˁ[
//----------------------------------------------------------------------------------------------------

#include"rock.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"create_filter.h"


//�e�N�X�`���̓��ꕨ
//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Rock_Texture = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Rock_Texture1 = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Rock_Texture2 = NULL;//�A���J�[�̃e�N�X�`��




rock::rock(b2Vec2 Position, float radius, int set_need_anchor_level)
{
	//�{�f�B�͈�Ł@�t�B�N�X�`�����t����\���ɂ���



	SetRockSize(b2Vec2(radius * 2, radius * 2));
	//SetAnchorPointSize(AnchorPoint_size);


	//radius�̕␳�������
	radius = radius / BOX2D_SCALE_MANAGEMENT;


	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//��ڂ̃{�f�B������

	//�T�C�Y��ݒ肷��
	b2Vec2 rock_size;
	rock_size.x = radius * 2 / BOX2D_SCALE_MANAGEMENT;
	rock_size.y = radius * 2 / BOX2D_SCALE_MANAGEMENT;


	b2BodyDef Rock_body;//�؂̊��̕���
	Rock_body.type = b2_dynamicBody;
	Rock_body.position.Set(Position.x, Position.y);
	Rock_body.fixedRotation = false;

	b2Body* m_Rock_body = world->CreateBody(&Rock_body);

	SetObjectAnchorPointBody(m_Rock_body);


	//��̃t�B�N�X�`��������
	// �~�`�̃V�F�C�v���`
	b2CircleShape circleShape;
	circleShape.m_radius = radius; // �~�̔��a��ݒ�

	b2FixtureDef rock_fixture;

	rock_fixture.shape = &circleShape;
	rock_fixture.density = 3.0f;
	rock_fixture.friction = 0.5f;//���C
	rock_fixture.restitution = 0.0f;//�����W��
	rock_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	rock_fixture.filter=createFilterExclude("object_filter", {});

	b2Fixture* object_rock_fixture = m_Rock_body->CreateFixture(&rock_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_rock_data = new ObjectData{ collider_object };//��U�ǔ���
	object_rock_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_rock_data);


	//2�߂̃t�B�N�X�`�����@��̒��ɃA���J�[�|�C���g������
	b2CircleShape anchorpoint_circlesShape;
	anchorpoint_circlesShape.m_radius = radius / 2;

	b2FixtureDef rock_anchorpoint_fixture;

	rock_anchorpoint_fixture.shape = &anchorpoint_circlesShape;
	rock_anchorpoint_fixture.density = 3.0f;
	rock_anchorpoint_fixture.friction = 0.5f;//���C
	rock_anchorpoint_fixture.restitution = 0.0f;//�����W��
	rock_anchorpoint_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	rock_anchorpoint_fixture.filter = createFilterExclude("object_filter", {});


	b2Fixture* object_rock_anchorpoint_fixture = m_Rock_body->CreateFixture(&rock_anchorpoint_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_rock_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_rock_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_rock_anchorpoint_data);

	object_rock_anchorpoint_data->object_name = Object_Rock;


	int ID = object_rock_anchorpoint_data->GenerateID();
	object_rock_anchorpoint_data->id = ID;
	SetID(ID);
	object_rock_anchorpoint_data->need_anchor_level = set_need_anchor_level;
	


	//���|�������ɕK�v�ɂȂ�Force �Ƃ肠�����T�C�Y�Ɉˑ��ł���
	b2Vec2 need_power;
	need_power.x = ((radius * radius) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 10;//�P�͕K�v�ɉ����ĕύX����
	need_power.y = 10.0f;//�c�ɕK�v�ȗ͂͂Ȃ�

	object_rock_anchorpoint_data->add_force = need_power;

};

rock::~rock()
{
}


void rock::Initialize()
{
	//�A���J�[�̕d�̕����i���{��j
	g_Rock_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_rock.png");
	g_Rock_Texture1 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
	g_Rock_Texture2 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");

}

void rock::Update()
{


}

void rock::Pulling_rock(b2Vec2 pulling_power)
{
	b2Body* body = GetObjectAnchorPointBody();
	//�v���C���[���ɓ|��
	if (PlayerPosition::GetPlayerPosition().x < body->GetPosition().x)//�v���C���[������
	{
		pulling_power.x = pulling_power.x * -1;
	}

	body->SetLinearVelocity(pulling_power);
	SetIfPulling(true);
}

void rock::Draw()
{

	///�������璲�����Ă�




	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 RockPos = GetObjectAnchorPointBody()->GetPosition();




	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((RockPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((RockPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Rock_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectAnchorPointBody()->GetAngle(),
		{ GetRockSize().x * scale,GetRockSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);




}

void rock::Finalize()
{
	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetObjectRockBody() != nullptr)
	{
		//�{�f�B�̍폜
		world->DestroyBody(Rock_body);
	}

	if (GetObjectAnchorPointBody() != nullptr)
	{
		world->DestroyBody(AnchorPoint_body);
	}
	//�摜�̉��
	UnInitTexture(g_Rock_Texture);
	UnInitTexture(g_Rock_Texture1);
	UnInitTexture(g_Rock_Texture2);
}