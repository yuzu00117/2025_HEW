//-----------------------------------------------------------------------------------------------------
// #name geyser.cpp
// #description �Ԍ���̃I�u�W�F�N�g��CPP
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//----------------------------------------------------------------------------------------------------

#include"geyser.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"sprite.h"
#include"texture.h"
#include"player_position.h"
#include"collider_type.h"
#include"player_position.h"
#include"create_filter.h"
#include"player.h"
#include"easing.h"


//�e�N�X�`���̓��ꕨ
//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Geyser_Texture = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Geyser_Water_Texture = NULL;//�A���J�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Geyser_on_Rock_Texture = NULL;//�A���J�[�̃e�N�X�`��




geyser::geyser(b2Vec2 GeyserPosition, b2Vec2 GeyserSize,b2Vec2 RangeFlyWaterSize)
{
	//�{�f�B�͈�Ł@�t�B�N�X�`�����t����\���ɂ���
	SetGeyserSize(GeyserSize);					//�Ԍ���̃T�C�Y���Z�b�g
	SetRangeFlyWaterSize(RangeFlyWaterSize);	//�Ԍ���̂������o�����̔�Ԕ͈�
	

	SetFlag(false);



	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//��ڂ̃{�f�B������ �Ԍ���

	b2Vec2 geyser_size;

	geyser_size.x = GeyserSize.x / BOX2D_SCALE_MANAGEMENT;
	geyser_size.y = GeyserSize.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef geyser_body;//�Ԍ���{�̂̃{�f�B
	geyser_body.type = b2_staticBody;
	geyser_body.position.Set(GeyserPosition.x, GeyserPosition.y);
	geyser_body.fixedRotation = true;//��]�͂��Ȃ�

	b2Body* m_geyser_body = world->CreateBody(&geyser_body);

	SetGeyserBody(m_geyser_body);



	//�Ԍ��򎩑̂̃t�B�N�X�`��
	b2PolygonShape geyser_shape;
	geyser_shape.SetAsBox(geyser_size.x * 0.5, geyser_size.y * 0.5);

	b2FixtureDef geyser_fixture;
	geyser_fixture.shape = &geyser_shape;
	geyser_fixture.density = 3.0f;
	geyser_fixture.restitution = 0.0f;//�����W��
	geyser_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	geyser_fixture.filter = createFilterExclude("object_filter", {});
	

	b2Fixture* object_geyser_fixture = m_geyser_body->CreateFixture(&geyser_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* geyser_object_data = new ObjectData{ collider_object };
	object_geyser_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(geyser_object_data);


	//�����o�����̃t�B�N�X�`����t����


	//�Ԍ���̐��̃T�C�Y�̃X�P�[���̒���
	b2Vec2 range_fly_water_size;
	range_fly_water_size.x = RangeFlyWaterSize.x / BOX2D_SCALE_MANAGEMENT;
	range_fly_water_size.y = RangeFlyWaterSize.y / BOX2D_SCALE_MANAGEMENT;

	//�Ԍ���̐��̃t�B�N�X�`��
	b2PolygonShape range_fly_water_shape;

	b2Vec2 vertices[4] = { b2Vec2(0.0f,0.0f) };

	vertices[0].Set(- range_fly_water_size.x/ 2, -geyser_size.y / 2);//����
	vertices[1].Set( range_fly_water_size.x / 2, -geyser_size.y / 2);//�E��
	vertices[2].Set(range_fly_water_size.x / 2, -geyser_size.y / 2-range_fly_water_size.y);//����
	vertices[3].Set(-range_fly_water_size.x / 2, -geyser_size.y / 2 - range_fly_water_size.y);//�E��

	range_fly_water_shape.Set(vertices, 4); // ���_���w�肵�ĎO�p�`��ݒ�

	b2FixtureDef range_fly_water_fixture;
	range_fly_water_fixture.shape = &range_fly_water_shape;
	range_fly_water_fixture.density = 3.0f;
	range_fly_water_fixture.restitution = 0.0f;//�����W��
	range_fly_water_fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	range_fly_water_fixture.filter = createFilterExclude("object_filter", {});


	b2Fixture* object_range_fly_water_fixture = m_geyser_body->CreateFixture(&range_fly_water_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* range_fly_water_object_data = new ObjectData{collider_geyser_water };//�ݒ���Ԍ���̐���
	object_range_fly_water_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(range_fly_water_object_data);

	range_fly_water_object_data->object_name = Object_Geyser_Water;
	

	range_fly_water_object_data->id = GetID();


};

geyser::~geyser()
{
}


void geyser::Initialize()
{
	//�Ԍ������̃e�N�X�`���I�I�I�i���{��j
	g_Geyser_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_gyaser.png");
	g_Geyser_Water_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_geyser_water.png");
	g_Geyser_on_Rock_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_geyser_on_rock.png");

}

void geyser::Update()
{
	JumpPlayer();
}


void geyser::JumpPlayer()
{
	if (GetFlag() == true)
	{
		//��ɏグ�鏊
		easing_rate += 0.02;

		if (1.5 < easing_rate)
		{
			easing_rate = 1.5;
		}
	
		// �C�[�W���O�����������l��ۑ�����ϐ�
		double easingRate;
		easingRate = Ease::InCubic(easing_rate);


		Player::GetOutSidePlayerBody()->ApplyForceToCenter(b2Vec2(0.0f, -easingRate), true);
		//-------------------------------------------------------------------------------------------

		b2Vec2 vec=Player::GetOutSidePlayerBody()->GetLinearVelocity();


		Player::GetOutSidePlayerBody()->SetLinearVelocity(b2Vec2(vec.x / 2, vec.y));




		
	}
	else
	{
		easing_rate =0.8;
	}
}

void geyser::Destroy_Joint()
{
	// ���[���h�̃C���X�^���X���擾
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	// �{�f�B�̃W���C���g���X�g���m�F
	b2Body* body = GetGeyserBody();
	if (body == nullptr) {
		return; // �{�f�B�������Ȃ牽�����Ȃ�
	}

	b2JointEdge* jointEdge = body->GetJointList();
	if (jointEdge == nullptr) {
		return; // �W���C���g�����݂��Ȃ��ꍇ�͉������Ȃ�
	}

	// �W���C���g��j��
	world->DestroyJoint(jointEdge->joint);
}

void geyser::PullingOnRock()
{
	float player_pos_x = PlayerPosition::GetPlayerPosition().x;
	float rock_pos_x = GetGeyserOnRockBody()->GetPosition().x;

	float add_force = GetGeyserOnRockSize().x * GetGeyserOnRockSize().y * 10;

	if (player_pos_x < rock_pos_x)
	{
		add_force = add_force * -1;
	}


	GetGeyserOnRockBody()->ApplyForceToCenter(b2Vec2(add_force,0.0f), true);
}


void geyser::Draw()
{

	///�������璲�����Ă�

	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 geyser_pos = GetGeyserBody()->GetPosition();




	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((geyser_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((geyser_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetGeyserBody()->GetAngle(),
		{ GetGeyserSize().x * scale,GetGeyserSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Geyser_Water_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y-(GetGeyserSize().y/2*scale)-(GetRangeFlyWaterSize().y/2*scale)},
		GetGeyserBody()->GetAngle(),
		{ GetRangeFlyWaterSize().x * scale,GetRangeFlyWaterSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);


}

void geyser::Finalize()
{
	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	if (GetGeyserBody() != nullptr)
	{
		//�{�f�B�̍폜
		world->DestroyBody(geyser_body);
	}

	if (GetGeyserOnRockBody() != nullptr)
	{
		world->DestroyBody(geyser_on_rock_body);
	}
	//�摜�̉��
	UnInitTexture(g_Geyser_on_Rock_Texture);
	UnInitTexture(g_Geyser_Texture);
	UnInitTexture(g_Geyser_Water_Texture);
}