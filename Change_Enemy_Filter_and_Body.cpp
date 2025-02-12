
//-----------------------------------------------------------------------------------------------------
// #name Chage_Enemy_filter_and_body.cpp
// #description �G�̃{�f�B�ƃt�B���^�[��V��������
// #make 2025/02/03�@�i��`��
// #update 2024/02/03
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------

#include"Change_Enemy_Filter_and_Body.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"


change_enemy_filter_and_body::change_enemy_filter_and_body(b2Vec2 position, b2Vec2 size, b2Vec2 velocity, ID3D11ShaderResourceView* Texture, int texture_x, int texture_y,b2Vec2 vec)
{
	SetSize(size);//�`��p�̃T�C�Y��ۑ�
	g_Texture = Texture;

	Splitting_x = texture_x;//���������ق�
	Splitting_y = texture_y;//���������ق�

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
	fixture.filter = createFilterExclude("blown_away_enemy_filter", {"Player_filter", "ground_filter","enemy_filter","one-way_platform_filter","object_filter","item_filter","Boss_filter" });

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// �V�����t�B���^�[���쐬


	//�J�X�^���f�[�^���쐬
	ObjectData* object_data = new ObjectData{ collider_blown_away_enemy };
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);

	int ID = object_data->GenerateID();
	object_data->id = ID;
	object_data->object_name = NULL_object;
	SetID(ID);


	//�����ɗ͂�������

	b2Vec2 Vec = vec;
	float angle;

	if (0,2>ReturnAbsoluteValue(Vec.x))
	{
		if (m_body->GetPosition().x < PlayerPosition::GetPlayerPosition().x)
		{
			Vec = b2Vec2(-0.3f, -0.5f);
		}
		else
		{
			Vec = b2Vec2(0.3f, -0.5f);
		}
	}


	if (Vec.y > 0)
	{
		Vec.y = -0.5f;
	}


	

	if (m_body->GetPosition().x < PlayerPosition::GetPlayerPosition().x)
	{
		angle = -5.0;
	}
	else
	{
		angle = 5.0;
	}

	m_body->ApplyLinearImpulseToCenter(b2Vec2(Vec.x, Vec.y),true);

	m_body->ApplyTorque(angle* size.x*size.y,true);
	


}

change_enemy_filter_and_body::~change_enemy_filter_and_body()
{
}



void change_enemy_filter_and_body::Initialize()
{

}

void change_enemy_filter_and_body::Update()
{
	
	if (m_body != nullptr)
	{
		if (sheet_cnt_now < max_sheet_cnt)
		{
			sheet_cnt_now += 0.5;
		}


		if (Destory_Flag == true)
		{
			// ���[���h�̃C���X�^���X���擾
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();

			if (m_body != nullptr)
			{
				world->DestroyBody(m_body); // �{�f�B���폜
				m_body = nullptr; // �|�C���^�𖳌������Ĉ��S�ɂ���
			}
		}


		enemy_eising_rate+=0.02;

		
	;
		enemy_size_scale = Ease::InCubic(enemy_eising_rate);
	}

}

void change_enemy_filter_and_body::DrawFront()
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
		DrawSplittingSprite(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale *enemy_size_scale,GetSize().y * scale* enemy_size_scale },
			Splitting_x,Splitting_y, sheet_cnt_now,3.0f
		);
	}

}

void change_enemy_filter_and_body::Finalize()
{
	UnInitTexture(g_Texture);
}