#include"Chage_Enemy_Filter_and_Body.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"

change_enemy_filter_and_body::change_enemy_filter_and_body(b2Vec2 position, b2Vec2 size, b2Vec2 velocity, ID3D11ShaderResourceView* Textur, float speed, float angle)
{
	SetSize(size);//�`��p�̃T�C�Y��ۑ�

	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//�T�C�Y�𒲐�����
	b2Vec2 body_size;
	body_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	body_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	//�{�f�B���쐬����
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(position.x, position.y);
	body.fixedRotation = false;

	b2Body* m_Body = world->CreateBody(&body);

	SetBody(m_Body);
	

	//�`�̒�`
	b2PolygonShape shape;
	shape.SetAsBox(body_size.x * 0.5, body_size.y * 0.5);

	//-----------------------------------------------------
	//	fixture�����
	b2FixtureDef fixture;

	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.01f;
	fixture.restitution = 0.3f;
	fixture.isSensor = false;
	fixture.filter = createFilterExclude("blown_away_enemy_filter", { "ground_filter","Player_filter","enemy_filter","one-way_platform_filter","object_filter","item_filter","Boss_filter" });

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// �V�����t�B���^�[���쐬


	//�J�X�^���f�[�^���쐬
	ObjectData* object_data = new ObjectData{ collider_blown_away_enemy };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = NULL_object;
	SetID(ID);




}

change_enemy_filter_and_body::~change_enemy_filter_and_body()
{
}



void change_enemy_filter_and_body::Initialize()
{

}

void change_enemy_filter_and_body::Update()
{
	




}

void change_enemy_filter_and_body::Draw()
{


	if (m_body != nullptr)
	{
		// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
		float scale = SCREEN_SCALE;

		// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;

		b2Vec2 Pos = GetBody()->GetPosition();


		float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);


		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale ,GetSize().y * scale }
		);
	}

}

void change_enemy_filter_and_body::Finalize()
{
	UnInitTexture(g_Texture);
}