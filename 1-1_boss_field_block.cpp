//-----------------------------------------------------------------------------------------------------
// #name boss_field_block.cpp
// #description �{�X�핔���̃t�B�[���h�𐶐�����CPP
// #make 2025/01/14�@�i��`��
// #update 2025/01/14
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------

#include"1-1_boss_field_block.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"

static ID3D11ShaderResourceView* g_Texture = NULL;//�t�B�[���h�̃e�N�X�`��

boss_field_block::boss_field_block(b2Vec2 position, b2Vec2 size, int block_hp, Boss_Room_Level level)
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
	body.type = b2_staticBody;
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
	fixture.restitution = 0.0f;
	fixture.isSensor = false;

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	//�J�X�^���f�[�^���쐬
	ObjectData* object_data = new ObjectData{ collider_object };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = NULL_object;
	SetID(ID);


	//�e�����ȕϐ����Z�b�g
	Block_Hp = block_hp;

	BossRoomLevel = level;



}

boss_field_block::~boss_field_block()
{
}



void boss_field_block::Initialize()
{
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");//�O���E���h�̃e�N�X�`��
}

void boss_field_block::Update()
{

}

void boss_field_block::Draw()
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

void boss_field_block::Finalize()
{
	UnInitTexture(g_Texture);
}