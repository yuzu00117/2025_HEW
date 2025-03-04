//----------------------------------------------------------------------------------------------------
// #name�@texture_blokc.cpp
// #description �w�i�ɕ`�悷��e�N�X�`��
// #make 2025/02/09
// #update 2025/02/09
// #comment �ǉ��E�C���\��  
//         
//          
//----------------------------------------------------------------------------------------------------
#include"texture_block.h"
#include"sprite.h"
#include"texture.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"player_position.h"
#include"create_filter.h"
#include"collider_type.h"
#include"game.h"
#include"player.h"







Texture_block::Texture_block(b2Vec2 Position, b2Vec2 block_size, float texture_angle, ID3D11ShaderResourceView* texture)
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

	b2Body* m_block_body = world->CreateBody(&body);

	SetBody(m_block_body);


	//�t�B�N�X�`����t����
	b2PolygonShape block_shape;

	block_shape.SetAsBox(size.x * 0.5, size.y * 0.5);

	b2FixtureDef block_fixture;

	block_fixture.shape = &block_shape;
	block_fixture.density = 3.0f;
	block_fixture.friction = 0.5f;//���C
	block_fixture.restitution = 0.0f;//�����W��
	block_fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	block_fixture.filter = createFilterExclude("texture_body_filter", { "Player_filter" ,"object_filter","enemy_filter","ground_filter" });

	b2Fixture* object_fixture = m_body->CreateFixture(&block_fixture);


	// ���j�[�N�|�C���^�[���g���� ObjectData ���쐬
	m_objectData = std::make_unique<ObjectData>(collider_UI_block);
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());


	int ID = object_data->GenerateID();
	object_data->id = ID;

	SetID(ID);


	
	angle = texture_angle * M_PI / 180.0;
	m_texture = texture;
};

Texture_block::~Texture_block()
{
	Finalize();
}


void Texture_block::Initialize()
{

}

void Texture_block::Update()
{

}



void Texture_block::Draw()
{

	if (GetBody() != nullptr)
	{

		///�������璲�����Ă�

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




		GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);
		//draw
		DrawSprite(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale,GetSize().y * scale }
		);

	}




}

void Texture_block::Finalize()
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