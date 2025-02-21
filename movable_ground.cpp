//-----------------------------------------------------------------------------------------------------
// #name movable_ground.cpp
// #description ��������鏰��CPP
// #make 2024/12/26�@���j�S
// #update 2024/12/26
// #comment �ǉ��E�C���\��
//         
//----------------------------------------------------------------------------------------------------
#include "movable_ground.h"
#include"texture.h"
#include"collider_type.h"
#include"create_filter.h"
#include"player_position.h"
#include"sprite.h"
#include"gokai.h"
#include"hit_stop.h"
#include"camera_shake.h"


//�e�N�X�`���̓��ꕨ
//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Ground_Texture = NULL;//���̃e�N�X�`���P
static ID3D11ShaderResourceView* g_Border_Texture_Lv1 = NULL;
static ID3D11ShaderResourceView* g_Border_Texture_Lv2 = NULL;
static ID3D11ShaderResourceView* g_Border_Texture_Lv3 = NULL;


bool	g_pulled = false;	//������������ꂽ���ǂ������擾�i���������u�Ԃň�������ꂽ�����ɂȂ�j

int		g_need_level = 1;	//Update�Ŏ����p�ɃA���J�[���x��������Ă���

movable_ground::movable_ground(b2Vec2 Position, b2Vec2 Ground_size, b2Vec2 AnchorPoint_size, int need_level)
{
	SetGroundSize(Ground_size);
	SetAnchorPointSize(AnchorPoint_size);



	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//----------------------------------------------------------------------------//
	//��ڂ̃{�f�B������

	//�T�C�Y��ݒ肷��
	b2Vec2 ground_size;
	ground_size.x = Ground_size.x / BOX2D_SCALE_MANAGEMENT;
	ground_size.y = Ground_size.y / BOX2D_SCALE_MANAGEMENT * 0.995f;


	b2BodyDef ground_body;//���̕���
	ground_body.type = b2_dynamicBody;
	ground_body.position.Set(Position.x, Position.y);
	ground_body.fixedRotation = true;

	b2Body* p_Ground_body = world->CreateBody(&ground_body);

	SetObjectGroundBody(p_Ground_body);



	b2PolygonShape ground_shape;
	ground_shape.SetAsBox(ground_size.x * 0.48, ground_size.y * 0.5);
	
	b2FixtureDef ground_fixture;

	ground_fixture.shape = &ground_shape;
	ground_fixture.density = 1.0f;
	ground_fixture.friction = 0.3f;//���C
	ground_fixture.restitution = 0.0f;//�����W��
	ground_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	ground_fixture.filter = createFilterExclude("object_filter", {});

	//�G���������ǂ����̃Z���T�[
	b2PolygonShape sensor_shape;
	b2Vec2 vertices[4];
	vertices[0].Set(-ground_size.x * 0.5f, -ground_size.y * 0.45f );	//����
	vertices[1].Set(-ground_size.x * 0.42f, -ground_size.y * 0.45f);		//�E��
	vertices[2].Set(-ground_size.x * 0.5f, ground_size.y * 0.45f);		//����
	vertices[3].Set(-ground_size.x * 0.42f, ground_size.y*0.45f );		//�E��
	sensor_shape.Set(vertices, 4);	//�Z���T�[�̃��[�J���ʒu��ύX

	b2FixtureDef sensor_fixture;

	sensor_fixture.shape = &sensor_shape;
	sensor_fixture.density = 1.0f;
	sensor_fixture.friction = 0.0f;//���C
	sensor_fixture.restitution = 0.0f;//�����W��
	sensor_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	sensor_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_ground_fixture = p_Ground_body->CreateFixture(&ground_fixture);
	b2Fixture* object_sensor_fixture = p_Ground_body->CreateFixture(&sensor_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_ground_data = new ObjectData{ collider_ground };//��U�ǔ���
	ObjectData* object_sensor_data = new ObjectData{ collider_object_destroyer_of_enemy };//��U�ǔ���
	object_ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_ground_data);
	object_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_sensor_data);
	object_ground_data->object_name = Object_Movable_Ground;

	int ID = object_ground_data->GenerateID();
	object_ground_data->id = ID;
	object_sensor_data->id = ID;
	SetID(ID);


	//---------------------------------------------------------------------------//
	//2�߂̃{�f�B�@���̏�̃A���J�[�|�C���g������

	//�T�C�Y�̕␳�������
	b2Vec2 anchorpoint_size;
	anchorpoint_size.x = AnchorPoint_size.x / BOX2D_SCALE_MANAGEMENT;
	anchorpoint_size.y = AnchorPoint_size.y / BOX2D_SCALE_MANAGEMENT;



	b2BodyDef anchorpoint_body;//�A���J�[�|�C���g�̕���
	anchorpoint_body.type = b2_dynamicBody;
	anchorpoint_body.position.Set(
		Position.x+ground_size.x / 2,
		Position.y + (ground_size.y / 2) + (anchorpoint_size.y / 2));
	anchorpoint_body.fixedRotation = true;

	b2Body* m_AnchorPoint_body = world->CreateBody(&anchorpoint_body);

	SetObjectAnchorPointBody(m_AnchorPoint_body);



	b2PolygonShape anchorpoint_shape;
	anchorpoint_shape.SetAsBox(anchorpoint_size.x * 0.5, anchorpoint_size.y * 0.5);

	b2FixtureDef anchorpoint_fixture;

	anchorpoint_fixture.shape = &anchorpoint_shape;
	anchorpoint_fixture.density = 1.0f;
	anchorpoint_fixture.friction = 0.05f;//���C
	anchorpoint_fixture.restitution = 0.0f;//�����W��
	anchorpoint_fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* object_anchorpoint_fixture = m_AnchorPoint_body->CreateFixture(&anchorpoint_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_anchorpoint_data = new ObjectData{ collider_anchor_point };
	object_anchorpoint_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_anchorpoint_data);

	object_anchorpoint_data->object_name = Object_Movable_Ground;


	object_anchorpoint_data->id = id;

	//�����������鎞�ɕK�v�ɂȂ�Force �Ƃ肠�����T�C�Y�Ɉˑ��ł���
	b2Vec2 need_power;

	need_power.x = ((GetGroundSize().x * GetGroundSize().y) + (GetAnchorPointSize().x * GetAnchorPointSize().y)) * 1.0;//�P�͕K�v�ɉ����ĕύX����
	need_power.y = 10.0f;//�c�ɕK�v�ȗ͂͂Ȃ�


	object_anchorpoint_data->add_force = need_power;
	m_pulling_power = object_anchorpoint_data->add_force;
	m_pulling_power = need_power;


	//�A���J�[���x���̐ݒ�
	object_anchorpoint_data->need_anchor_level = need_level;
	g_need_level = need_level;

	//�A���J�[���x���������o�ϐ��ŕێ�
	m_need_level = need_level;


	//-----------------------------------------------------------------------------------------------------------------------------------------
	//�W���C���g����

	b2WeldJointDef jointDef;
	jointDef.bodyA = p_Ground_body;
	jointDef.bodyB = m_AnchorPoint_body;
	jointDef.localAnchorA.Set(-ground_size.x * 0.5f, -ground_size.y * 0.5f); // ���̏�[
	jointDef.localAnchorB.Set(0.0, anchorpoint_size.y * 0.5f); // �A���J�[�|�C���g�̉��[
	jointDef.collideConnected = false;					  //�W���C���g�������̓��m�̐ڐG������

	world->CreateJoint(&jointDef);						  //���[���h�ɃW���C���g��ǉ�

	//-------------------------------------------------------------------------------------------
	//������������ׂɕK�v�ȋ���

}

movable_ground::~movable_ground()
{
}

void movable_ground::Initialize()
{
	g_Ground_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
	g_Border_Texture_Lv1 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");
	g_Border_Texture_Lv2 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_yellow.png");
	g_Border_Texture_Lv3 = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
}

void movable_ground::Update()
{
	if (pulling)
	{
		Pulling_ground();

		int cunt_enemy = 0;		//�G���C�|�������̃J�E���g
		//�G��|������
		if (Ground_body->GetLinearVelocity().x != 0.0f)
		{

			for (auto w : enemy_static)
			{
				w->CollisionPulledObject();
				app_atomex_start(Player_Dead_Sound);
				HitStop::StartHitStop(15);
				CameraShake::StartCameraShake(5, 3, 15);
				cunt_enemy++;
			}
			enemy_static.clear();

			for (auto w : enemy_dynamic)
			{
				w->CollisionPulledObject();
				app_atomex_start(Player_Dead_Sound);
				HitStop::StartHitStop(15);
				CameraShake::StartCameraShake(5, 3, 15);
				cunt_enemy++;
			}
			enemy_dynamic.clear();

			for (auto w : enemy_floating)
			{
				w->CollisionPulledObject();
				app_atomex_start(Player_Dead_Sound);
				HitStop::StartHitStop(15);
				CameraShake::StartCameraShake(5, 3, 15);
				cunt_enemy++;
			}
			enemy_floating.clear();
		}
		
		//�����x�֘A
		//=================================================
		switch (g_need_level)
		{
		case 1:
			Gokai_UI::AddGokaiCount(100 * cunt_enemy);
			break;
		case 2:
			Gokai_UI::AddGokaiCount(500 * cunt_enemy);
			break;
		case 3:
			Gokai_UI::AddGokaiCount(1000 * cunt_enemy);
			break;
		default:
			break;
		}


		//�����������Ȃ����߂̏���
		//=============================================================================================
		if (Ground_body->GetLinearVelocity().x > 0)
		{
			Ground_body->SetLinearVelocity({ 0.0f,0.0f });
			g_pulled = true;
		}
	}
}


void movable_ground::Draw()
{
	///�������璲�����Ă�




	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 GroundPos = GetObjectGroundBody()->GetPosition();




	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((GroundPos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((GroundPos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;
	
	b2Vec2 size = GetGroundSize();

	if (m_is_border)
	{
		switch (m_need_level)
		{
		case 1:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Border_Texture_Lv1);
			break;
		case 2:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Border_Texture_Lv2);
			break;
		case 3:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Border_Texture_Lv3);
			break;
		default:
			break;
		}
		DrawSprite(
			{ draw_x,
			  draw_y },
			GetObjectAnchorPointBody()->GetAngle(),
			{ GetGroundSize().x * scale*1.1f,GetGroundSize().y * scale*1.1f }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
			, m_border_alpha
		);

		//���ߗ��ݒ�
		m_border_alpha -= 0.01;
		if (m_border_alpha <= m_border_alpha_min)
		{
			m_border_alpha = m_border_alpha_max;
		}
	}

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Ground_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetObjectAnchorPointBody()->GetAngle(),
		{ GetGroundSize().x * scale,GetGroundSize().y * scale }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
	);

}

void movable_ground::Finalize()
{
	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//�{�f�B�̍폜
	world->DestroyBody(Ground_body);
	world->DestroyBody(AnchorPoint_body);

	//�e�N�X�`���̉��

	if (g_Ground_Texture != NULL)
	{
		UnInitTexture(g_Ground_Texture);
		UnInitTexture(g_Border_Texture_Lv1);
		UnInitTexture(g_Border_Texture_Lv2);
		UnInitTexture(g_Border_Texture_Lv3);

		g_Ground_Texture = NULL;
		g_Border_Texture_Lv1 = NULL;
		g_Border_Texture_Lv2 = NULL;
		g_Border_Texture_Lv3 = NULL;
	}


}

void movable_ground::Pulling_ground()
{
	b2Body* body = GetObjectAnchorPointBody();
	b2Vec2 pulling_power = m_pulling_power;

	//�v���C���[���Ɉ�������
	if (PlayerPosition::GetPlayerPosition().x < body->GetPosition().x )//�v���C���[������
	{
		pulling_power.x = pulling_power.x * -8;
	}
	else
	{
		pulling_power.x = 0.0f;
	}

	body->SetLinearVelocity(pulling_power);

	//���̕`��I��
	m_is_border = false;
}

bool movable_ground::GetIfPulled()
{
	return g_pulled;
}

