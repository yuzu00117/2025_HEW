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
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"

static ID3D11ShaderResourceView* g_Texture = NULL;//�t�B�[���h�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Top_Texture = NULL;//�t�B�[���h�̃e�N�X�`��

boss_field_block::boss_field_block(b2Vec2 position, b2Vec2 size, int block_hp, Boss_Room_Level level,int texture_type)
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
	fixture.restitution = 0.3f;
	fixture.isSensor = false;
	fixture.filter = createFilterExclude("ground_filter",{});

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// �V�����t�B���^�[���쐬


	//�J�X�^���f�[�^���쐬
	ObjectData* object_data = new ObjectData{ collider_ground};
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = Boss_field_block;
	SetID(ID);


	//�e�����ȕϐ����Z�b�g
	Block_Hp = block_hp;

	BossRoomLevel = level;

	Texture_type = texture_type;


}

boss_field_block::~boss_field_block()
{
}



void boss_field_block::Initialize()
{
	if (g_Texture == NULL) {
		g_Texture =		InitTexture(L"asset\\texture\\stage_block\\iseki_block.png");						//��Ղ̒��̃e�N�X�`��
		g_Top_Texture=  InitTexture(L"asset\\texture\\stage_block\\iseki_block_top.png");				//��Ղ̏�̃e�N�X�`��
	}
}

void boss_field_block::Update()
{
	Boss_1_1& boss = Boss_1_1::GetInstance();

		if (boss.GetBossFieldLevel() > BossRoomLevel && break_flag == false)
		{
			m_body->SetType(b2_dynamicBody);

			// �t�B���^�[��ύX
			b2Fixture* fixture = m_body->GetFixtureList();

			// �t�B�N�X�`�������݂��Ȃ��ꍇ�͑������^�[��
			if (!fixture) {
				return;
			}

			// �V�����t�B���^�[���쐬
			b2Filter newFilter = createFilterExclude("Boss_field_filter", { "ground_filter","object_filter","Boss_filter" });
			fixture->SetFilterData(newFilter);

			break_flag = true;

			int x = GetRandomInt(1, 10);


			int minus = GetRandomInt(0, 1);

			if (minus == 1)
			{
				minus = -1;
			}
			else
			{
				minus = 1;
			}

			m_body->ApplyLinearImpulseToCenter(b2Vec2(x / 5 * minus, 0.0f), true);

		}

		if (break_flag == true)
		{
			body_delete_cnt++;
		}

		//�R�b�Ԃ�������{�f�B������
		if (body_delete_cnt > 180)
		{
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();

			if (m_body == nullptr)
			{
				world->DestroyBody(m_body);
			}

		}
	




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


		if (Texture_type != 0)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
		}
		else
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Top_Texture);
		}


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
	if (g_Texture != NULL)
	{
		UnInitTexture(g_Texture);
		g_Texture = NULL;
		UnInitTexture(g_Top_Texture);
		g_Top_Texture = NULL;
	}
}