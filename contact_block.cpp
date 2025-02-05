// #name�@�ڐG������
// #description �ڐG������w�肵���ꏊ�ɔ�ԃu���b�N
// #make 2024/01/02
// #update 2024/01/02
// #comment �ǉ��E�C���\��
//         
//          
//----------------------------------------------------------------------------------------------------

#include"contact_block.h"
#include"sprite.h"
#include"texture.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"player_position.h"
#include"create_filter.h"
#include"collider_type.h"
#include"game.h"
#include"player.h"


static ID3D11ShaderResourceView* g_Texture = NULL;//�n�ʂ̃e�N�X�`��

//�ڐG�������ɗl�X�ȏo�������N����
contact_block::contact_block(b2Vec2 Position, b2Vec2 block_size, Contact_Block_Type type,b2Vec2 num)
{
	//�T�C�Y���Z�b�g
	SetSize(block_size);

	b2Vec2 size;

	size.x = block_size.x / BOX2D_SCALE_MANAGEMENT;
	size.y = block_size.y / BOX2D_SCALE_MANAGEMENT;






	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();





	b2BodyDef body;//�؂̊��̕���
	body.type = b2_staticBody;
	body.position.Set(Position.x, Position.y);
	body.fixedRotation = true;

	b2Body* m_teleport_block_body = world->CreateBody(&body);

	SetBody(m_teleport_block_body);


	//�t�B�N�X�`����t����
	b2PolygonShape teleport_block_shape;

	teleport_block_shape.SetAsBox(size.x * 0.5, size.y * 0.5);

	b2FixtureDef teleport_block_fixture;

	teleport_block_fixture.shape = &teleport_block_shape;
	teleport_block_fixture.density = 3.0f;
	teleport_block_fixture.friction = 0.5f;//���C
	teleport_block_fixture.restitution = 0.0f;//�����W��
	teleport_block_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	teleport_block_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_teleport_fixture = m_body->CreateFixture(&teleport_block_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_teleport_data = new ObjectData{ collider_contact_block };//��U�ǔ���
	object_teleport_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_teleport_data);




	int ID = object_teleport_data->GenerateID();
	object_teleport_data->id = ID;

	SetID(ID);


	m_contact_type = type;


};

contact_block::~contact_block()
{
}


void contact_block::Initialize()
{
	//�A���J�[�̕d�̕����i���{��j
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");
	

}

void contact_block::Update()
{
	if (m_body != nullptr)
	{
		if (m_flag==true)
		{
			Player& player = Player::GetInstance();
			switch (m_contact_type)
			{
			case NULL_TYPE:
				break;
			case DEAD_BLOCK_TYPE:

				

				//�v���C���[����������
				player.Player_Damaged(-1000, 0);

				break;
			default:
				break;
			}


			//�t���O�����Z�b�g
			m_flag = false;
		}

	}
}



void contact_block::Draw()
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


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ GetSize().x * scale,GetSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);




}

void contact_block::Finalize()
{
	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetBody() != nullptr)
	{
		//�{�f�B�̍폜
		world->DestroyBody(m_body);
	}

	//�摜�̉��

	if (g_Texture != NULL)
	{
		UnInitTexture(g_Texture);
		g_Texture = NULL;
	}

}