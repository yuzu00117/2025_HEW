//-----------------------------------------------------------------------------------------------------
// #name 1-1_boss.cpp
// #description boss������@������
// #make 2025/01/07�@�@�i��`��
// #update 2025/01/21
// #comment �ǉ��E�C���\��
//          �E���ɂȂ�
//----------------------------------------------------------------------------------------------------
#include"1-1_boss.h"
#include"include/box2d/box2d.h"
#include"texture.h"
#include"sprite.h"
#include"main.h"
#include"world_box2d.h"
#include"create_filter.h"
#include"player_position.h"
#include"display.h"
#include"collider_type.h"


// �g�p����e�N�X�`���t�@�C�����i�[
static ID3D11ShaderResourceView* g_mini_boss_Texture = NULL;//�����ȃS�[�����̃e�N�X�`��
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet1_Texture = NULL;//�Ռ��g�̃e�N�X�`���P
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet2_Texture = NULL;//�Ռ��g�̃e�N�X�`���Q
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet1_Texture = NULL;//���ߍU���̃e�N�X�`���P
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet2_Texture = NULL;//���ߍU���̃e�N�X�`���Q
static ID3D11ShaderResourceView* g_mini_boss_create_sheet1_Texture = NULL;//�����ȃS�[�����𐶐�����ۂ̃{�X���P
static ID3D11ShaderResourceView* g_mini_boss_create_sheet2_Texture = NULL;//�����ȃS�[�����𐶐�����ۂ̃{�X���Q
static ID3D11ShaderResourceView* g_mini_boss_walk_sheet1_Texture = NULL;//�S�[�����̕������[�V�����P
static ID3D11ShaderResourceView* g_mini_boss_walk_sheet2_Texture = NULL;//�S�[�����̕������[�V�����Q



//-------------------------------------------------------------------------------------------
//�f�o�b�N�p�̉摜
static ID3D11ShaderResourceView* g_debug_color = NULL;//�f�o�b�N�p


Boss_1_1::Boss_1_1()
{



}

Boss_1_1::~Boss_1_1()
{
}

void Boss_1_1::Initialize(b2Vec2 position, b2Vec2 bodysize,bool left)
{
	g_mini_boss_Texture = InitTexture(L"asset\\texture\\boss_1_1\\mini_boss.png");//�~�j�S�[�����̃C���N���[�h
	g_boss_shock_wave_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet1.png");//�Ռ��g�U���̃C���N���[�h�V�[�g�P
	g_boss_shock_wave_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet2.png");//�Ռ��g�U���̃C���N���[�h�V�[�g�Q
	g_boss_charge_attack_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet1.png");//���ߍU���̃V�[�g�P
	g_boss_charge_attack_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet2_.png");//���ߍU���̃V�[�g�Q
	g_mini_boss_create_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet1.png");//�~�j�S�[�����̐����̃{�X���P
	g_mini_boss_create_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet2_.png");//�~�j�S�[�����̐����̃{�X���Q
	g_mini_boss_walk_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet1.png");//�S�[�����̕������[�V�����P
	g_mini_boss_walk_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet2.png");//�S�[�����̕������[�V�����Q


	//�f�o�b�N�p
	g_debug_color = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");//�~�j�S�[�����̐����̃{�X���Q


	//�{�f�B�̃T�C�Y���Z�b�g
	SetBossDrawSize(bodysize);

	b2Vec2 size; //�T�C�Y�̃X�P�[���𒲐�
	size.x = bodysize.x / BOX2D_SCALE_MANAGEMENT;
	size.y = bodysize.y / BOX2D_SCALE_MANAGEMENT;

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(position.x, position.y);
	body.angle = 0.0f;
	body.fixedRotation = true;//��]���Œ�ɂ���
	body.userData.pointer = (uintptr_t)this;


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();

	//���[���h�ɓo�^
	b2Body* m_boss_body = world->CreateBody(&body);

	SetBossBody(m_boss_body);


	b2PolygonShape body_shape;




	b2Vec2 vertices[4] = { b2Vec2(0.0f,0.0f) };

	if (left = true)
	{
		// �����v���Œ��_��ݒ�
		vertices[0].Set(-size.x / 3, size.y / 2);  // ����
		vertices[1].Set(size.x / 2, size.y / 2);   // �E��
		vertices[2].Set(size.x / 2, 0.0);    // �E��
		vertices[3].Set(-size.x / 3, 0.0);   // ����
	}
	else
	{
		// �����v���Œ��_��ݒ�
		vertices[0].Set(-size.x / 2, size.y / 2);  // ����
		vertices[1].Set(size.x / 3, size.y / 2);   // �E��
		vertices[2].Set(size.x / 3, 0.0);    // �E��
		vertices[3].Set(-size.x / 2, 0.0);   // ����
	}


	body_shape.Set(vertices, 4);

	b2FixtureDef body_fixture;
	body_fixture.shape = &body_shape;
	body_fixture.friction = 0.0f;//���C
	body_fixture.restitution = 0.1f;//�����W��
	body_fixture.density = 0.1f;
	body_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����


	b2Fixture* m_body_fixture = m_body->CreateFixture(&body_fixture);


	now_boss_state = charge_attack_state;

}

void Boss_1_1::Update()
{
	if (m_body != nullptr)
	{

		

		//���E�̐U����������
		float player_x= PlayerPosition::GetPlayerPosition().x;
		if (player_x < m_body->GetPosition().x)//���ɂ���
		{
			left_flag = true;
		}
		else
		{
			left_flag = false;
		}

		if (old_left_flag != left_flag)
		{
			int i = 0;
		}

		old_left_flag - left_flag;


		switch (now_boss_state)
		{
		case wait_state:
			break;
		case walk_state:
			sheet_cnt += 0.5;
			if (Max_Walk_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
			}

			m_body->SetLinearVelocity(b2Vec2(-0.5f, 0.0f));


			break;
		case shock_wave_state:
			sheet_cnt += 0.5;
			if (Max_Shock_Wave_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = create_mini_golem_state;
			}


			break;
		case create_mini_golem_state:
			sheet_cnt += 0.5;
			if (Max_Create_Mini_Golem_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = charge_attack_state;
			}


			break;
		case charge_attack_state:
			sheet_cnt += 0.75;



			//�@�����̎��ɉ�ʂ�U���̂�����
			if (68 < sheet_cnt && sheet_cnt < 140)
			{
				if (display_shake_flag == true)
				{
				
					display_shake_flag = false;
				}
				else
				{
			
					display_shake_flag = true;
				}
			}
			//���[�V��������������
			if (Max_Charge_Attack_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = shock_wave_state;
			}
			break;
		default:
			break;

		}
	}
}

void Boss_1_1::CreateChargeAttack(b2Vec2 attack_size, bool left)
{
	if (GetAttackBody() == nullptr) {

		//�{�f�B�̃T�C�Y���Z�b�g
		SetAttackDrawSize(attack_size);

		b2Vec2 size; //�T�C�Y�̃X�P�[���𒲐�
		size.x = attack_size.x / BOX2D_SCALE_MANAGEMENT;
		size.y = attack_size.y / BOX2D_SCALE_MANAGEMENT;

		b2BodyDef body;
		body.type = b2_dynamicBody;

		b2Vec2 boss_pos = m_body->GetPosition();
		b2Vec2 boss_size = GetBossDrawSize();

		if (left) {
			body.position.Set(boss_pos.x - (boss_size.x / 3) - (attack_size.x / 2), boss_pos.y);
		}
		else
		{
			body.position.Set(boss_pos.x + (boss_size.x / 3) + (attack_size.x / 2), boss_pos.y);
		}
		body.angle = 0.0f;
		body.fixedRotation = true;//��]���Œ�ɂ���
		body.userData.pointer = (uintptr_t)this;


		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		//���[���h�ɓo�^
		b2Body* m_attack_body = world->CreateBody(&body);

		SetAttackBody(m_attack_body);

		//�ʏ�U���̃t�B�N�X�`��
		b2FixtureDef fixture;

		// �N���X���� b2Shape �������o�[�Ƃ��ĕێ�����ꍇ�̗�
		b2PolygonShape shape; // �N���X�̃����o�[�ϐ��Ƃ��ĕێ�
		shape.SetAsBox(attack_size.x * 0.5, attack_size.y * 0.5);

		fixture.shape = &shape;//�`��ݒ�
		fixture.density = 0.1f;//���x
		fixture.friction = 0.0f;//���C
		fixture.restitution = 0.0f;//�����W��
		fixture.isSensor = false;//�Z���T�[���ǂ���

		b2Fixture* m_fixture = m_body->CreateFixture(&fixture);


		ObjectData* boss_attack_data = new ObjectData{ collider_enemy_dynamic };
		m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_attack_data);

		//�v���C���[�ƃW���C���g����
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;//�{�X�̃{�f�B
		jointDef.bodyB = GetAttackBody();//boss�̍U���̃{�f�B

		if (left)//�E���ǂ���
		{
			//boss��
			jointDef.localAnchorA.Set((-boss_size.x * 0.5), 0.0f);
			//�U����
			jointDef.localAnchorB.Set((attack_size.x * 0.5), 0.0f);
		}
		else//����
		{
			//boss��
			jointDef.localAnchorA.Set((boss_size.x * 0.5), 0.0f);
			//�U����
			jointDef.localAnchorB.Set((-attack_size.x * 0.5), 0.0f);
		}
		jointDef.collideConnected = true;//�W���C���g�������̓��m�̐ڐG������

		world->CreateJoint(&jointDef); //���[���h�ɃW���C���g��ǉ�

	}

}

void Boss_1_1::DeleteAttackBody()
{
	if (GetAttackBody() != nullptr)
	{

		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		b2Body* m_destroy_body = GetAttackBody();

		world->DestroyBody(m_destroy_body);

		SetAttackBody(nullptr);

	}
}

void Boss_1_1::Draw()
{
	if (m_body != nullptr)
	{
		float scale = SCREEN_SCALE;

		// �X�N���[�������ʒu (16m x 9m �̉𑜓x�ŁA������ x = 8, y = 4.5 �Ɖ���)
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;



		// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
		b2Vec2 boss_pos = GetBossBody()->GetPosition();

		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;




	

		switch (now_boss_state)
		{
		case wait_state:
			break;
		case shock_wave_state:
			// �V�F�[�_���\�[�X��ݒ�
			if (sheet_cnt < Max_Shock_Wave_Sheet / 2) {
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_shock_wave_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 7, 7, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_shock_wave_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 7, 7, sheet_cnt - Max_Shock_Wave_Sheet / 2, boss_alpha, left_flag);
			}


			break;
		case create_mini_golem_state:
			if (sheet_cnt < Max_Create_Mini_Golem_Sheet / 2) {
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_create_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 7, 7, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_create_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 7, 7, sheet_cnt - Max_Create_Mini_Golem_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		case charge_attack_state:
			if (sheet_cnt < Max_Charge_Attack_Sheet / 2) {
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_charge_attack_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 10, 10, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_charge_attack_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 10, 10, sheet_cnt - Max_Charge_Attack_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		case walk_state://�������[�V����

			if (sheet_cnt < Max_Walk_Sheet / 2) {
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_walk_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_walk_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt - Max_Walk_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		default:
			break;
		}






		debugDraw();
	}
}
void Boss_1_1::debugDraw()
{


	///------------------------------------------------------------------------
	//�{�f�B�̂����蔻���\��

	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (16m x 9m �̉𑜓x�ŁA������ x = 8, y = 4.5 �Ɖ���)
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_color);

	// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
	b2Vec2 boss_pos = GetBossBody()->GetPosition();

	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



	float pos_y = PlayerPosition::GetPlayerPosition().y;
	DrawSprite(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale));
}

void Boss_1_1::Finalize()
{

}