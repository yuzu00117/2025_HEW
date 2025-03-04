// #name�@break_block.cpp
// #description �I�u�W�F�N�g�ƐڐG���������u���b�N
// #make 2025/02/06
// #update 2025/02/06
// #comment �ǉ��E�C���\��  �����Ȋ���������
//         
//          
//----------------------------------------------------------------------------------------------------

#include"break_block.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"
#include"break_effect.h"
#include"camera_shake.h"


//�I�u�W�F�N�g�ɐG�ꂽ�����I�u�W�F�N�g
Break_Block::Break_Block(b2Vec2 Position, b2Vec2 block_size, int divisions_x, int divisions_y, float angle, ID3D11ShaderResourceView* g_Texture)
{
	//�T�C�Y���Z�b�g
	SetSize(block_size);
	

	b2Vec2 size;

	size.x = block_size.x / BOX2D_SCALE_MANAGEMENT;
	size.y = block_size.y / BOX2D_SCALE_MANAGEMENT;


	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();



	b2BodyDef body;//�{�f�B
	body.type = b2_staticBody;
	body.position.Set(Position.x, Position.y);
	body.fixedRotation = true;
	body.angle = angle * M_PI / 180.0;

	b2Body* m_block_body = world->CreateBody(&body);

	SetBody(m_block_body);


	//�t�B�N�X�`����t����
	b2PolygonShape block_shape;

	block_shape.SetAsBox(size.x * 0.5, size.y * 0.5);

	b2FixtureDef block_fixture;

	block_fixture.shape = &block_shape;
	block_fixture.density = 1.0f;
	block_fixture.friction = 0.5f;//���C
	block_fixture.restitution = 0.0f;//�����W��
	block_fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	block_fixture.filter = createFilterExclude("break_block_body_filter", {});

	b2Fixture* object_fixture = m_body->CreateFixture(&block_fixture);

	

	// ���j�[�N�|�C���^�[���g���� ObjectData ���쐬
	m_objectData = std::make_unique<ObjectData>(collider_break_block);
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());

	b2FixtureDef player_fixture;

	player_fixture.shape = &block_shape;
	player_fixture.density = 1.0f;
	player_fixture.friction = 0.5f;//���C
	player_fixture.restitution = 0.0f;//�����W��
	player_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	player_fixture.filter = createFilterExclude("break_block_body_filter", {"object_filter"});

	b2Fixture* m_player_fixture = m_body->CreateFixture(&player_fixture);

	// ���j�[�N�|�C���^�[���g���� ObjectData ���쐬
	m_player_objectData = std::make_unique<ObjectData>(collider_break_block);
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_player_objectData.get());



	int ID = m_objectData->GenerateID();
	m_objectData->id = ID;
	m_player_objectData->id = ID;

	SetID(ID);


	Texture = g_Texture;
	

};

Break_Block::~Break_Block()
{
	Finalize();
}


void Break_Block::Initialize()
{


}

void Break_Block::Update()
{
	if (m_body != nullptr)
	{
		if (m_flag == true)
		{
			PillarFragmentsManager::GetInstance().Destroy_Splitting(m_body, Texture, GetSize());

			SetBody(nullptr);

			CameraShake::StartCameraShake(20, 20, 20);

			m_flag = false;
		}

	}
}



void Break_Block::Draw()
{

	///�������璲�����Ă�

	if (m_body != nullptr)
	{
		// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
		float scale = SCREEN_SCALE;

		// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;

		b2Vec2 Pos = GetBody()->GetPosition();




		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		GetDeviceContext()->PSSetShaderResources(0, 1, &Texture);

		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			angle,
			{ GetSize().x * scale,GetSize().y * scale }
		);

	}



}

void Break_Block::Finalize()
{
	if (!m_body) return;

	for (b2Fixture* fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
		if (!fixture) continue;

		// UserData �擾


		// �����ȃ|�C���^�Ȃ�X�L�b�v
		if (!fixture->GetUserData().pointer) {
			continue;
		}



		// ObjectData ���폜��
		fixture->GetUserData().pointer = 0;  // �|�C���^�̃N���A
	}

	// `b2Body` ���폜
	Box2dWorld::GetInstance().GetBox2dWorldPointer()->DestroyBody(m_body);
	m_body = nullptr;
}