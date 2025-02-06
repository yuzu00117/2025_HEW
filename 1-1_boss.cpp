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
#include"keyboard.h"
#include"1_1boss_state_debug.h"
#include"scene.h"
#include"anchor.h"
#include"camera_shake.h"
#include"hit_stop.h"
#include"Item_Spirit.h"


// �g�p����e�N�X�`���t�@�C�����i�[
static ID3D11ShaderResourceView* g_mini_boss_Texture = NULL;//�����ȃS�[�����̃e�N�X�`��
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet1_Texture = NULL;//�Ռ��g�̃e�N�X�`���P
static ID3D11ShaderResourceView* g_boss_shock_wave_sheet2_Texture = NULL;//�Ռ��g�̃e�N�X�`���Q
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet1_Texture = NULL;//���ߍU���̃e�N�X�`���P
static ID3D11ShaderResourceView* g_boss_charge_attack_sheet2_Texture = NULL;//���ߍU���̃e�N�X�`���Q

static ID3D11ShaderResourceView* g_boss_walk_sheet1_Texture = NULL;//�S�[�����̕������[�V�����P
static ID3D11ShaderResourceView* g_boss_walk_sheet2_Texture = NULL;//�S�[�����̕������[�V�����Q
static ID3D11ShaderResourceView* g_boss_jump_sheet1_Texture = NULL;//�S�[�����̃W�����v���[�V�����P
static ID3D11ShaderResourceView* g_boss_jump_sheet2_Texture = NULL;//�S�[�����̃W�����v���[�V����2
static ID3D11ShaderResourceView* g_boss_panic_sheet_Texture = NULL;//�S�[�����̍�����e���[�V����

static ID3D11ShaderResourceView* g_mini_boss_create_sheet1_Texture = NULL;//�����ȃS�[�����𐶐�����ۂ̃{�X���P
static ID3D11ShaderResourceView* g_mini_boss_create_sheet2_Texture = NULL;//�����ȃS�[�����𐶐�����ۂ̃{�X���Q


//�{�X�܂��̃G�t�F�N�g�̃e�N�X�`��
static ID3D11ShaderResourceView* g_boss_charge_effect = NULL;//�{�X�̂��ߒ��̃G�t�F�N�g
static ID3D11ShaderResourceView* g_boss_charge_attack_effect = NULL;//�{�X�̂��ߍU���̔���̃G�t�F�N�g
static ID3D11ShaderResourceView* g_mini_golem_break_effect = NULL;//�����ȃS�[������j�󂷂�Ƃ��ɂł�G�t�F�N�g
static ID3D11ShaderResourceView* g_boss_panic_effect = NULL;//�{�X����e�����Ƃ��̃G�t�F�N�g
static ID3D11ShaderResourceView* g_boss_shock_wave_effect = NULL;//�{�X�̏Ռ��g�U��




//-------------------------------------------------------------------------------------------
//�f�o�b�N�p�̉摜
static ID3D11ShaderResourceView* g_debug_color = NULL;//�f�o�b�N�p

static ID3D11ShaderResourceView* g_debug_boss_body_color = NULL;//�f�o�b�N�p

static ID3D11ShaderResourceView* g_debug_attack_color = NULL;//�f�o�b�N�p

static ID3D11ShaderResourceView* g_debug_core = NULL;//�f�o�b�N�p






//���Ƃ�CPP����Q�Ƃł��邽��
b2Body* outside_boss_body;

Boss_1_1::Boss_1_1()
{



}

Boss_1_1::~Boss_1_1()
{
}



void Boss_1_1::Initialize(b2Vec2 position, b2Vec2 bodysize, bool left)
{
	//ボディのサイズ
	bodysize.x=bodysize.x* BOSS_SIZE_SCALE;
	bodysize.y=bodysize.y * BOSS_SIZE_SCALE;

	if (g_mini_boss_create_sheet1_Texture == NULL)
	{
		g_mini_boss_Texture = InitTexture(L"asset\\texture\\boss_1_1\\mini_boss.png");//�~�j�S�[�����̃C���N���[�h
		g_boss_shock_wave_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet1.png");//�Ռ��g�U���̃C���N���[�h�V�[�g�P
		g_boss_shock_wave_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_sheet2.png");//�Ռ��g�U���̃C���N���[�h�V�[�g�Q
		g_boss_charge_attack_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet1.png");//���ߍU���̃V�[�g�P
		g_boss_charge_attack_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_sheet2_.png");//���ߍU���̃V�[�g�Q
		g_mini_boss_create_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet1.png");//�~�j�S�[�����̐����̃{�X���P
		g_mini_boss_create_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_create_mini_golem_sheet2_.png");//�~�j�S�[�����̐����̃{�X���Q
		g_boss_walk_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet1.png");//�S�[�����̕������[�V�����P
		g_boss_walk_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_walk_sheet2.png");//�S�[�����̕������[�V�����Q
		g_boss_jump_sheet1_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_jump_new_sheet1.png");//�S�[�����̃W�����v���[�V�����P
		g_boss_jump_sheet2_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_jump_new_sheet2.png");//�S�[�����̃W�����v���[�V�����Q
		g_boss_panic_sheet_Texture = InitTexture(L"asset\\texture\\boss_1_1\\boss_panic_sheet1.png");//�S�[�����̋��݃��[�V�����P


		//�G�t�F�N�g

		g_boss_charge_attack_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_attack_effect.png");	//�{�X�̃`���[�W�U�����̃G�t�F�N�g
		g_boss_charge_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_charge_effect.png");				//�{�X�̃`���[�W�U�����̃G�t�F�N�g
		g_boss_panic_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_panic_effect.png");					//�{�X�̃`���[�W�U�����̃G�t�F�N�g
		g_boss_shock_wave_effect = InitTexture(L"asset\\texture\\boss_1_1\\boss_shock_wave_effect.png");		//�{�X�̃`���[�W�U�����̃G�t�F�N�g
		g_mini_golem_break_effect = InitTexture(L"asset\\texture\\boss_1_1\\mini_golem_break_effect.png");		//�{�X�̃`���[�W�U�����̃G�t�F�N�g

		//�f�o�b�N�p
		g_debug_color = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");//sensor�̃e�N�X�`��
		g_debug_boss_body_color = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");
		g_debug_attack_color = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
		g_debug_core = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_blue.png");


		InitializeBossDebug();//�f�o�b�N�p�̂���

	}
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//�{�f�B�������������
	if (GetBossBody() != nullptr)
	{
		world->DestroyBody(GetBossBody());
		SetBossBody(nullptr);
	}

	//�{�f�B�̃T�C�Y���Z�b�g
	SetBossDrawSize(bodysize);


	//���ۂ̃{�X�̃T�C�Y
	bodysize.x = bodysize.x * 0.5;
	bodysize.y = bodysize.y * 0.4;

	SetBossRealSize(bodysize);

	//boss�̃T�C�Y
	b2Vec2 size; //�T�C�Y�̃X�P�[���𒲐�
	size.x = bodysize.x / BOX2D_SCALE_MANAGEMENT;
	size.y = bodysize.y / BOX2D_SCALE_MANAGEMENT;




	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(position.x, position.y);
	body.angle = 0.0f;
	body.fixedRotation = true;//��]���Œ�ɂ���
	body.userData.pointer = (uintptr_t)this;




	//���[���h�ɓo�^
	b2Body* m_boss_body = world->CreateBody(&body);

	SetBossBody(m_boss_body);
	outside_boss_body = m_boss_body;//�O������̎Q�Ƃ悤�u

	b2PolygonShape body_shape;







	body_shape.SetAsBox(size.x * 0.5, size.y * 0.5);


	b2FixtureDef body_fixture;
	body_fixture.shape = &body_shape;
	body_fixture.friction = 0.3f;//���C
	body_fixture.restitution = 0.1f;//�����W��
	body_fixture.density = 0.1f;
	body_fixture.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	body_fixture.filter = createFilterExclude("Boss_filter", {});


	b2Fixture* m_body_fixture = m_body->CreateFixture(&body_fixture);

	ObjectData* boss_body_data = new ObjectData{ collider_boss };
	m_body_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_body_data);


	//�Z���T�[�̃T�C�Y���L�^
	SetBossSensorSize(b2Vec2(bodysize.x * 2.0, bodysize.y));

	b2Vec2 sensor_size;
	sensor_size.x = bodysize.x / BOX2D_SCALE_MANAGEMENT * 2.0;
	sensor_size.y = bodysize.y / BOX2D_SCALE_MANAGEMENT;


	b2FixtureDef sensor_fixture;
	b2PolygonShape sensor_shape;

	b2Vec2 sensor_vertices[4] = { b2Vec2(0.0f,0.0f) };



	sensor_shape.SetAsBox(sensor_size.x * 0.5, sensor_size.y * 0.5);

	sensor_fixture.shape = &sensor_shape;
	sensor_fixture.friction = 0.0f;//���C
	sensor_fixture.restitution = 0.1f;//�����W��
	sensor_fixture.density = 0.1f;
	sensor_fixture.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����



	b2Fixture* m_sensor_fixture = m_body->CreateFixture(&sensor_fixture);

	ObjectData* boss_sensor_data = new ObjectData{ collider_boss_senosr };
	m_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_sensor_data);


	boss_field_level = 1;
}

void Boss_1_1::Update()
{
	if (m_body != nullptr)
	{

		//---------------------------------------------------------------------------------------------------------------------------
		//���E�̐U����������
		float player_x = PlayerPosition::GetPlayerPosition().x;
		if (player_x < m_body->GetPosition().x)//���ɂ���
		{
			left_flag = true;
		}
		else
		{
			left_flag = false;
		}


		//-------------------------------------------------------------------------------------------

		//�f�o�b�N�����̍X�V
		UpdateBossDebug();

		//�Ռ��g�̍X�V����
		ShockWaveUpdate();

		//�~�j�S�[�����̍X�V����
		MiniGolemUpdate();
		//�~�j�S�[����
		DestroyMiniGolemBody();
		//�W�����v�̍X�V����
		JumpUpdate();

		//�{�X�̃R�A�̍X�V����
		BossCoreUpdate();

		//�{�X�����񂾂�
		BossDead();



		//�N�[���^�C���̊Ǘ�
		UpdateCoolTime();

		//�G�t�F�N�g�̊Ǘ�
		UpdateEffectSheetCnt();


		//�{�X�̋ߋ��������������̔���Ɏg��
		if (GetPlayerNearbylocked() != 0)
		{
			SetPlayerNearbylocked(GetPlayerNearbylocked() - 1);
		}


		//-------------------------------------------------------------------------------------------
		// �f�o�b�N�p�@�{�Ԋ��ł͂���

		//�A���J�[�|�C���g�̃{�f�B���폜����f�o�b�N�p
		if (Keyboard_IsKeyDown(KK_Y) && debug_flag == 0)
		{
			debug_flag = 60;
			boss_field_level++;
		}
		if (debug_flag != 0)
		{
			debug_flag--;
		}

		//-------------------------------------------------------------------------------------------


		switch (now_boss_state)
		{
		case wait_state:

			sheet_cnt = 0;

			break;
		case panic_state:



			CreateBossCore(b2Vec2(2.0f*BOSS_SIZE_SCALE, 2.0f * BOSS_SIZE_SCALE));

			sheet_cnt += 0.5;

			if (Max_Panic_Stun_Frame <= sheet_cnt)
			{
				sheet_cnt = 0;
				CoreDeleteFlag = true;

			}

			break;
		case walk_state:
			if (static_cast<int>(sheet_cnt) % 10 == 0)
			{

				if (left_flag)
				{
					m_body->SetLinearVelocity(b2Vec2(-walk_power.x, walk_power.y));
				}
				else
				{
					m_body->SetLinearVelocity(b2Vec2(walk_power.x, walk_power.y));
				}

				if ((static_cast<int>(sheet_cnt) % 20 == 0))
				{
					m_body->SetLinearVelocity(b2Vec2_zero);
				}

				//�J�����V�F�C�N�X�^�[�g
				CameraShake::StartCameraShake(1, 0, 10);

			}
			sheet_cnt += 0.5;

			if (Max_Walk_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}


			break;

		case jump_state:
			sheet_cnt += 0.6;

			if (Max_Jump_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;

				//�W�����v�̂��ƒn���ׂ肷�邩�� X���̉����x���O��Y�͂��̂܂�
				b2Body* body = GetBossBody();
				b2Vec2 velocity = body->GetLinearVelocity();
				body->SetLinearVelocity(b2Vec2(0.0f, velocity.y));


				//�J�����V�F�C�N�X�^�[�g
				CameraShake::StartCameraShake(120, 00, 20);
				HitStop::SetHitStopFlag(5);

				now_boss_state = wait_state;
			}

			break;
		case shock_wave_state:
			sheet_cnt += 0.5;

			if (static_cast<int>(sheet_cnt) == Shock_Wave_Start_Frame)
			{
				CreateShockWave(b2Vec2(5.0f * BOSS_SIZE_SCALE, 6.0f * BOSS_SIZE_SCALE), left_flag);
				Shock_Wave_Fly_flag = true;

				//�G�t�F�N�g�X�^�[�g
				shock_wave_effect_sheet_cnt = 1;
			}






			if (Max_Shock_Wave_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}


			break;
		case create_mini_golem_state:
			sheet_cnt += 0.5;

			if (static_cast<int>(sheet_cnt) == Create_Mini_Golem_Start_Frame)
			{
				CreateMiniGolem(b2Vec2(3.0f * BOSS_SIZE_SCALE, 2.0f * BOSS_SIZE_SCALE), left_flag);//�摜�����ɋ󔒂����邽�߁@���������L�΂��@���ۂ̔��a�ŎQ�Ƃ��Ă���̂͂���


			}
			if (Max_Create_Mini_Golem_Sheet <= sheet_cnt)
			{
				Mini_golem_Create_flag = true;//�~�j�S�[�����̃N���G�C�g�t���O�̊Ǘ�
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}


			break;
		case charge_attack_state:

			//�V�[�g�P���ڂ���͐i�s����������
			if (sheet_cnt < 100)
			{
				sheet_cnt += 0.75;
			}
			else // �V�[�g�Q���ڂ���͐i�s�������x��
			{
				sheet_cnt += 0.5;
			}

			if (static_cast<int>(sheet_cnt) == Charge_Attack_Start_Frame)
			{
				CreateChargeAttack(b2Vec2(4.0f * BOSS_SIZE_SCALE, 4.0f * BOSS_SIZE_SCALE), left_flag);
				//�G�t�F�N�g�X�^�[�g
				charge_attack_effect_sheet_cnt = 1;
			}
			if (static_cast<int>(sheet_cnt) == Charge_Attack_End_Frame)
			{
				DeleteAttackBody();
			}

			//���[�V��������������
			if (Max_Charge_Attack_Sheet <= sheet_cnt)
			{
				sheet_cnt = 0;
				now_boss_state = wait_state;
			}
			break;

		default:
			break;

		}
	}
}

void Boss_1_1::UpdateCoolTime(void)
{
	Now_Shock_Wave_CoolTime++;		//�Ռ��g�U���̃C���N�������g
	Now_Jump_CoolTime++;			//�W�����v�̃C���N�������g
	Now_Create_MiniGolem_CoolTime++;//�~�j�S�[�����̃C���N�������g
	Now_Charge_Attack_CoolTime++;	//�`���[�W�U���̃C���N�������g
	Now_Max_Walk_CoolTime++;		//�������[�V����


	if (Wait_time == 0)
	{
		//�ߋ�������������
		if (now_boss_state == wait_state)
		{
			if (Player_is_Nearby == true)
			{
				//�ߋ����ɂ���

				//�`���[�W�U��
				if (Now_Charge_Attack_CoolTime > Max_Charge_Attack_CoolTime)
				{
					now_boss_state = charge_attack_state;
					Now_Charge_Attack_CoolTime = 0;
					return;
				}

				//�W�����v
				if (Now_Jump_CoolTime > Max_Jump_CoolTime)
				{
					now_boss_state = jump_state;
					Now_Jump_CoolTime = 0;
					return;
				}

			}
			else
			{
			


				//�Ռ��g�U��
				if (Now_Shock_Wave_CoolTime > Max_Shock_Wave_CoolTime)
				{
					now_boss_state = shock_wave_state;
					Now_Shock_Wave_CoolTime = 0;
					return;
				}

				//���␶��
				if (Now_Create_MiniGolem_CoolTime > Max_Create_MiniGolem_CoolTime)
				{
					now_boss_state = create_mini_golem_state;
					Now_Create_MiniGolem_CoolTime = 0;
					return;
				}


				//�����Ȃ�������������[�V����
				if (wait_state == now_boss_state)
				{
					now_boss_state = walk_state;
				}



			}
		}
	}
	else
	{
		Wait_time--;
	}
}

void Boss_1_1::BossDamaged(void)
{
	//�{�X��HP�����炷����ɂ��`�ԕύX����
	SetBossHP(GetBossHP() - 1);
	SetWaitCoolTime(120);

}

void Boss_1_1::BossDead(void)
{
	//�{�X��HP���O�ȉ��ɂȂ����烊�U���g�ɔ��
	if (boss_hp <= 0)
	{
		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_RESULT);
	}
}

void Boss_1_1::BossCoreUpdate()
{
	if (CoreDeleteFlag == true)
	{
		DestroyBossCore();//�{�X�̃R�A��j��

		sheet_cnt = 0;//�V�[�g�J�E���g�����Z�b�
		CoreDeleteFlag = false;

	}
}


void Boss_1_1::CreateBossCore(b2Vec2 size)
{

	if (GetAnchorPointBody() == nullptr)
	{
		//�T�C�Y���Z�b�g
		SetAnchorPointSize(size);

		b2Vec2 anchorpoint_size;

		anchorpoint_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
		anchorpoint_size.y = size.y / BOX2D_SCALE_MANAGEMENT;



		b2Vec2 position = m_body->GetPosition();

		b2BodyDef anchor_point_body;
		anchor_point_body.type = b2_dynamicBody;//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�true
		anchor_point_body.position.Set(position.x, position.y);			//�|�W�V�������Z�b�g
		anchor_point_body.angle = 0;									//�p�x�̒�`
		anchor_point_body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
		anchor_point_body.fixedRotation = true;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�


		Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
		b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���

		b2Body* m_anchor_point_body = world->CreateBody(&anchor_point_body);//Body�����[���h�ɌŒ�

		SetAnchorPointBody(m_anchor_point_body);


		b2PolygonShape shape;                         //shape�ɂ͐F�X�Ȍ^������@�T�[�N���Ƃ��������
		shape.SetAsBox(anchorpoint_size.x * 0.5f, anchorpoint_size.y * 0.5f);//�����蔻���o�^����4�_�@*0.5����̂�

		b2FixtureDef fixture;
		fixture.shape = &shape;    //�V���[�v���t�B�N�X�`���ɓo�^����
		fixture.density = 1.0f;    //���x
		fixture.friction = 0.05f;  //���C
		fixture.restitution = 0.0f;//�����W��
		fixture.isSensor = false;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

		b2Fixture* ground_fixture = m_anchor_point_body->CreateFixture(&fixture);//Body���Ƀt�B�N�X�`����o�^����

		// �J�X�^���f�[�^���쐬���Đݒ�
		// �n�ʂɒl��o�^
		// �n�ʂɃ��[�U�[�f�[�^��o�^
		ObjectData* data = new ObjectData{ collider_anchor_point };
		ground_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);


		data->need_anchor_level = 1;

		data->object_name = Boss_core;



		//�v���C���[�ƃW���C���g����
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;//�{�X�̃{�f�B
		jointDef.bodyB = GetAnchorPointBody();//boss�̍U���̃{�f�B

		//boss��
		jointDef.localAnchorA.Set(0.0f, 0.0f);
		//�U����
		jointDef.localAnchorB.Set(0.0f, 0.0f);

		jointDef.collideConnected = true;//�W���C���g�������̓��m�̐ڐG������

		world->CreateJoint(&jointDef); //���[���h�ɃW���C���g��ǉ�


		//�G�t�F�N�g�X�^�[�g
		panic_effect_sheet_cnt = 1;

		//�J�����V�F�C�N�X�^�[�g
		CameraShake::StartCameraShake(40, 20, 40);
		HitStop::SetHitStopFlag(10);


		b2Vec2 vec = m_body->GetLinearVelocity();

		m_body->SetLinearVelocity(b2Vec2(0.0f, vec.y));

		sheet_cnt = 0;
	}
}

void Boss_1_1::DestroyBossCore(void)
{
	//�A���J�[�|�C���g�{�f�B�������
	if (GetAnchorPointBody() != nullptr)
	{
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
		b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���



		world->DestroyBody(GetAnchorPointBody());

		//�e�N�X�`�����I��
		panic_effect_sheet_cnt = 0;

		//最初の行動はチャージ攻撃にしたい
		Now_Charge_Attack_CoolTime += 1500;

		now_boss_state = wait_state;

		//null���Z�b�g
		SetAnchorPointBody(nullptr);
	}
}

void Boss_1_1::JumpUpdate()
{
	if (now_boss_state == jump_state)
	{
		if (Jump_Start_Frame <= sheet_cnt && sheet_cnt <= Jump_End_Frame)
		{
			b2Body* boss_body = GetBossBody();

			int minus_flag = 1;

			if (left_flag)
			{
				minus_flag = -1;
			}

			boss_body->SetLinearVelocity(b2Vec2(minus_flag * JumpPower.x, JumpPower.y));
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
		b2Vec2 boss_size = b2Vec2(GetBossRealSize().x / BOX2D_SCALE_MANAGEMENT, GetBossRealSize().y / BOX2D_SCALE_MANAGEMENT);

		if (left) {
			body.position.Set(boss_pos.x - (boss_size.x / 3) - (size.x / 2), boss_pos.y + boss_size.y / 2 - size.y / 2);
		}
		else
		{
			body.position.Set(boss_pos.x + (boss_size.x / 3) + (size.x / 2), boss_pos.y + boss_size.y / 2 - size.y / 2);
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
		shape.SetAsBox(size.x * 0.5, size.y * 0.5);

		fixture.shape = &shape;//�`��ݒ�
		fixture.density = 0.1f;//���x
		fixture.friction = 0.0f;//���C
		fixture.restitution = 0.0f;//�����W��
		fixture.isSensor = true;//�Z���T�[���ǂ���

		b2Fixture* m_fixture = m_attack_body->CreateFixture(&fixture);


		ObjectData* boss_attack_data = new ObjectData{ collider_chage_attack };
		m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_attack_data);

		//�v���C���[�ƃW���C���g����
		b2WeldJointDef jointDef;
		jointDef.bodyA = m_body;//�{�X�̃{�f�B
		jointDef.bodyB = GetAttackBody();//boss�̍U���̃{�f�B

		if (left)//�E���ǂ���
		{
			//boss��
			jointDef.localAnchorA.Set((-boss_size.x / 3), boss_size.y / 2 - size.y / 2);
			//�U����
			jointDef.localAnchorB.Set((size.x * 0.5), 0.0f);
		}
		else//����
		{
			//boss��
			jointDef.localAnchorA.Set((boss_size.x / 3), boss_size.y / 2 - size.y / 2);
			//�U����
			jointDef.localAnchorB.Set((-size.x * 0.5), 0.0f);
		}
		jointDef.collideConnected = true;//�W���C���g�������̓��m�̐ڐG������

		world->CreateJoint(&jointDef); //���[���h�ɃW���C���g��ǉ�


		//�n�ʂ�j��
		boss_field_level++;

		//�J�����V�F�C�N�X�^�[�g
		CameraShake::StartCameraShake(40, 0, 60);
		HitStop::SetHitStopFlag(15);



	}

}


void Boss_1_1::CreateShockWave(b2Vec2 attack_size, bool left)
{
	if (GetAttackBody() == nullptr) {

		//�{�f�B�̃T�C�Y���Z�b�g
		SetAttackDrawSize(attack_size);

		b2Vec2 size; //�T�C�Y�̃X�P�[���𒲐�
		size.x = attack_size.x / BOX2D_SCALE_MANAGEMENT;
		size.y = attack_size.y / BOX2D_SCALE_MANAGEMENT;

		b2BodyDef body;
		body.type = b2_dynamicBody;
		body.gravityScale = (0.0f);//�d�͂̉e���������Ȃ�

		b2Vec2 boss_pos = m_body->GetPosition();
		b2Vec2 boss_size = b2Vec2(GetBossRealSize().x / BOX2D_SCALE_MANAGEMENT, GetBossRealSize().y / BOX2D_SCALE_MANAGEMENT);

		if (left) {
			body.position.Set(boss_pos.x - (boss_size.x / 3) - (size.x / 2), boss_pos.y + boss_size.y / 2 - size.y / 2);
			ShockWaveLeftFlag = true;
		}
		else
		{
			body.position.Set(boss_pos.x + (boss_size.x / 3) + (size.x / 2), boss_pos.y + boss_size.y / 2 - size.y / 2);
			ShockWaveLeftFlag = false;
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
		shape.SetAsBox(size.x * 0.5, size.y * 0.5);

		fixture.shape = &shape;//�`��ݒ�
		fixture.density = 0.0f;//���x
		fixture.friction = 0.0f;//���C
		fixture.restitution = 0.0f;//�����W��
		fixture.isSensor = true;//�Z���T�[���ǂ���

		b2Fixture* m_fixture = m_attack_body->CreateFixture(&fixture);


		ObjectData* boss_attack_data = new ObjectData{ collider_shock_wave };
		m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(boss_attack_data);

		//�J�����V�F�C�N�X�^�[�g
		CameraShake::StartCameraShake(0, 20, 40);
		HitStop::SetHitStopFlag(5);
	}
}


void Boss_1_1::ShockWaveUpdate(void)
{
	if (Shock_Wave_Fly_flag == true)
	{
		if (GetAttackBody() != nullptr)
		{
			float minus_flag = 1;
			if (ShockWaveLeftFlag == true)
			{
				minus_flag = -1;
			}

			GetAttackBody()->SetLinearVelocity(b2Vec2(minus_flag * Shock_Wave_Speed, 0.0f));
		}
		Now_Shock_Wave_time_Frame++;

		//�Ռ��g�̏I��
		if (Shock_Wave_time_Frame < Now_Shock_Wave_time_Frame)
		{
			DeleteAttackBody();
			Shock_Wave_Fly_flag = false;//���Z�b�g����
			Now_Shock_Wave_time_Frame = 0;//���Z�b�g����
		}
	}
}



void Boss_1_1::CreateMiniGolem(b2Vec2 mini_golem_size, bool left)
{
	for (int i = 0; i < 2; i++)
	{
		if (GetMiniGolemBody(i) == nullptr && Mini_golem_Create_flag == true) {

			//�{�f�B�̃T�C�Y���Z�b�g
			SetMiniGolemDrawSize(mini_golem_size);

			b2Vec2 size; //�T�C�Y�̃X�P�[���𒲐�
			size.x = mini_golem_size.x / BOX2D_SCALE_MANAGEMENT;
			size.y = mini_golem_size.y / BOX2D_SCALE_MANAGEMENT;

			b2BodyDef body;
			body.type = b2_dynamicBody;
			b2Vec2 boss_pos = m_body->GetPosition();
			b2Vec2 boss_size = b2Vec2(GetBossDrawSize().x / BOX2D_SCALE_MANAGEMENT, GetBossDrawSize().y / BOX2D_SCALE_MANAGEMENT);

			if (left) {
				body.position.Set(boss_pos.x, boss_pos.y);
			}
			else
			{
				body.position.Set(boss_pos.x, boss_pos.y);
			}
			body.angle = 0.0f;
			body.fixedRotation = false;//��]���Œ�ɂ���
			body.userData.pointer = (uintptr_t)this;



			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();

			//���[���h�ɓo�^
			b2Body* m_mini_golem_body = world->CreateBody(&body);

			SetMiniGolemBody(m_mini_golem_body, i);

			//�~�j�S�[�����̃t�B�N�X�`��
			b2FixtureDef fixture;

			// �N���X���� b2Shape �������o�[�Ƃ��ĕێ�����ꍇ�̗�
			b2CircleShape shape; // �N���X�̃����o�[�ϐ��Ƃ��ĕێ�
			shape.m_radius = size.y / 2;//�摜�����ɋ󔒂����邽�߁@�����Q��

			fixture.shape = &shape;//�`��ݒ�
			fixture.density = 1.0f;//���x
			fixture.friction = 0.5f;//���C
			fixture.restitution = 0.3f;//�����W��
			fixture.isSensor = false;//�Z���T�[���ǂ���
			fixture.filter = createFilterExclude("MiniGolem_filter", { "Boss_filter" });

			b2Fixture* m_fixture = m_mini_golem_body->CreateFixture(&fixture);


			ObjectData* mini_golem_data = new ObjectData{ collider_mini_golem };
			m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(mini_golem_data);

			Mini_golem_Create_flag = false;

			return;
		}

	}
}

void Boss_1_1::MiniGolemUpdate(void)
{
	for (int i = 0; i < 2; i++)
	{
		if (GetMiniGolemBody(i) != nullptr)
		{
			//�{�f�B�������Ă���
			b2Body* mini_golem_body = GetMiniGolemBody(i);

			//�v���C���[�̈ʒu���̒�`
			b2Vec2 player_pos = PlayerPosition::GetPlayerPosition();


			//�v���C���[�ƍ��E�ǂ���ɍs�����̒���
			if (player_pos.x < mini_golem_body->GetPosition().x)//�v���C���[�̕�����
			{
				if (mini_golem_body->GetAngularVelocity() > -3)//�ő��]�ʂ𐧌�
				{
					mini_golem_body->ApplyTorque(-0.1, true);
				}

			}
			else
			{
				if (mini_golem_body->GetAngularVelocity() < 3)//�ő��]�ʂ𐧌�
				{
					mini_golem_body->ApplyTorque(0.1, true);
				}
			}
		}
	}
}

void Boss_1_1::DestroyMiniGolemBody(void)
{
	if (destroy_mini_golem_flag == true)
	{
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();

		b2Body* m_mini_golem_body = destroy_mini_golem_body;

		//ソウルを落とす
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(m_mini_golem_body->GetPosition(), { 1.0f,2.0f }, 0.0f, ENEMY_GOLEM_SOULGAGE);


		//�쐬�G�t�F�N�g�p�̊Ǘ�
		mini_golem_delete_effect_position = m_mini_golem_body->GetPosition();
		mini_golem_break_effect_cnt = 1;

		world->DestroyBody(m_mini_golem_body);

		for (int i = 0; i < 2; i++)
		{
			if (GetMiniGolemBody(i) == destroy_mini_golem_body)
			{
				SetMiniGolemBody(nullptr, i);
				destroy_mini_golem_flag = false;


				//�J�����V�F�C�N�X�^�[�g
				CameraShake::StartCameraShake(0, 20, 10);
				HitStop::SetHitStopFlag(5);
			}
		}
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

		//�����̕\��
		DrawBossDebug();
		/*debugDraw();*/

		float scale = SCREEN_SCALE;

		// �X�N���[�������ʒu (16m x 9m �̉𑜓x�ŁA������ x = 8, y = 4.5 �Ɖ���)
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;



		// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
		b2Vec2 boss_pos = GetBossBody()->GetPosition();
		b2Vec2 real_boss_size;
		real_boss_size.x = GetBossRealSize().x / BOX2D_SCALE_MANAGEMENT;
		real_boss_size.y = GetBossRealSize().y / BOX2D_SCALE_MANAGEMENT;



		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y - (real_boss_size.y * 0.7)) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		switch (now_boss_state)
		{
		case wait_state:

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_walk_sheet1_Texture);

			DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, 1, boss_alpha, left_flag);

			break;

		case panic_state:

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_panic_sheet_Texture);

			DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 16, 17, sheet_cnt, boss_alpha, left_flag);

			break;
		case jump_state:
			// �V�F�[�_���\�[�X��ݒ�
			if (sheet_cnt < Max_Jump_Sheet / 2)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_jump_sheet1_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_jump_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt - Max_Jump_Sheet / 2, boss_alpha, left_flag);
			}
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
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_walk_sheet1_Texture);

				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt, boss_alpha, left_flag);
			}
			else
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_walk_sheet2_Texture);
				DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt - Max_Walk_Sheet / 2, boss_alpha, left_flag);
			}
			break;

		default:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_attack_color);
			DrawDividedSpriteBoss(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale), 6, 6, sheet_cnt - Max_Walk_Sheet / 2, boss_alpha, left_flag);
			break;
		}


		//�R�A���̕`��
		if (GetAnchorPointBody() != nullptr)
		{
			//�V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_attack_color);

			// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
			b2Vec2 anchorpoint_pos = GetAnchorPointBody()->GetPosition();

			// �v���C���[�ʒu���l�����ăX�N���[���␳��������
			//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
			float anchor_point_draw_x = ((anchorpoint_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float anchor_point_draw_y = ((anchorpoint_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			DrawSprite(XMFLOAT2(anchor_point_draw_x, anchor_point_draw_y), 0.0f, XMFLOAT2(GetAnchorPointSize().x * scale, GetAnchorPointSize().y * scale));

		}

		//----------------------------------------------------------------------------------------
		//�~�j�S�[������Draw
		for (int i = 0; i < 2; i++)
		{
			if (GetMiniGolemBody(i) != nullptr)
			{

				//�V�F�[�_���\�[�X��ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_boss_Texture);

				// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
				b2Vec2 mini_golem_pos = GetMiniGolemBody(i)->GetPosition();

				// �v���C���[�ʒu���l�����ăX�N���[���␳��������
				//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
				float mini_golem_draw_x = ((mini_golem_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float mini_golem_draw_y = ((mini_golem_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

				DrawSprite(XMFLOAT2(mini_golem_draw_x, mini_golem_draw_y), GetMiniGolemBody(i)->GetAngle(), XMFLOAT2(GetMiniGolemDrawSize().x * scale, GetMiniGolemDrawSize().y * scale));
			}
		}
	}
	EffectDraw();


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




	DrawSprite(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossDrawSize().x * scale, GetBossDrawSize().y * scale));

	DrawSprite(XMFLOAT2(draw_x, draw_y), 0.0f, XMFLOAT2(GetBossSensorSize().x * scale, GetBossSensorSize().y * scale));

	//----------------------------------------------------------------------------
	//���ۂ̃{�f�B�̃T�C�Y��\��

	if (GetBossBody() != nullptr)
	{

		//�V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_boss_body_color);

		// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
		b2Vec2 boss_pos = GetBossBody()->GetPosition();

		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float boss_draw_x = ((boss_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float boss_draw_y = ((boss_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		DrawSprite(XMFLOAT2(boss_draw_x, boss_draw_y), 0.0f, XMFLOAT2(GetBossRealSize().x * scale, GetBossRealSize().y * scale));
	}
	//---------------------------------------------------------------------------
	if (GetAttackBody() != nullptr)
	{


		//�V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_debug_attack_color);

		// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
		b2Vec2 attack_pos = GetAttackBody()->GetPosition();

		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float attack_draw_x = ((attack_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float attack_draw_y = ((attack_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

		DrawDividedSpriteBoss(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale, GetAttackDrawSize().y * scale), 6, 4, 1, 1.0, left_flag);

	}





}

void Boss_1_1::UpdateEffectSheetCnt()
{

	//�s���s��
	//if (panic_effect_sheet_cnt != 0)
	//{
	//	panic_effect_sheet_cnt += 0.5;

	//	if (Max_panic_effect_sheet_cnt<panic_effect_sheet_cnt)
	//	{
	//		panic_effect_sheet_cnt = 0;
	//	}

	//	
	//}

	//�`���[�W�U���̍U�����̃G�t�F�N�g
	if (charge_attack_effect_sheet_cnt != 0)
	{
		charge_attack_effect_sheet_cnt += 0.5;

		if (Max_charge_attack_effect_sheet_cnt < charge_attack_effect_sheet_cnt)
		{
			charge_attack_effect_sheet_cnt = 0;
		}


	}

	//�`���[�W��
	if (charge_effect_sheet_cnt != 0)
	{
		charge_effect_sheet_cnt += 0.5;

		if (Max_charge_effect_sheet_cnt < charge_effect_sheet_cnt)
		{
			charge_effect_sheet_cnt = 0;
		}


	}

	//�Ռ��g�U��
	if (shock_wave_effect_sheet_cnt != 0)
	{
		shock_wave_effect_sheet_cnt += 0.5;

		if (Max_shock_wave_effect_sheet_cnt < shock_wave_effect_sheet_cnt)
		{
			shock_wave_effect_sheet_cnt = 0;
		}


	}


	//mini�S�[�����̔j��
	if (mini_golem_break_effect_cnt != 0)
	{
		mini_golem_break_effect_cnt++;

		if (Max_mini_golem_break_effect < mini_golem_break_effect_cnt)
		{
			mini_golem_break_effect_cnt = 0;
		}
	}

}

void Boss_1_1::EffectDraw()
{

	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (16m x 9m �̉𑜓x�ŁA������ x = 8, y = 4.5 �Ɖ���)
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;
	//---------------------------------------------------------------------------
	//�`���[�W�U��
	if (GetAttackBody() != nullptr)
	{
		if (now_boss_state == charge_attack_state)
		{
			if (charge_attack_effect_sheet_cnt != 0)
			{
				//�V�F�[�_���\�[�X��ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_charge_attack_effect);

				// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
				b2Vec2 attack_pos;
				attack_pos.x = GetAttackBody()->GetPosition().x;
				attack_pos.y = GetAttackBody()->GetPosition().y + (GetAttackDrawSize().y / BOX2D_SCALE_MANAGEMENT / 2);

				// �v���C���[�ʒu���l�����ăX�N���[���␳��������
				//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
				float attack_draw_x = ((attack_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float attack_draw_y = ((attack_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;





				DrawDividedSpriteBoss(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale * 7, GetAttackDrawSize().y * scale * 7), 5, 6, charge_attack_effect_sheet_cnt, effect_alpha, left_flag);
			}

		}
		else//�V���b�N�E�F�[�u
		{
			if (shock_wave_effect_sheet_cnt != 0)
			{
				//�V�F�[�_���\�[�X��ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_shock_wave_effect);

				// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
				b2Vec2 attack_pos = GetAttackBody()->GetPosition();

				// �v���C���[�ʒu���l�����ăX�N���[���␳��������
				//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
				float attack_draw_x = ((attack_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
				float attack_draw_y = ((attack_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


				//���������X�v���C�g�����������΂���������C��
				bool left = 1;
				if (ShockWaveLeftFlag)
				{
					left = 0;
				}

				DrawDividedSpriteBoss(XMFLOAT2(attack_draw_x, attack_draw_y), 0.0f, XMFLOAT2(GetAttackDrawSize().x * scale * 1.3, GetAttackDrawSize().y * scale * 1.3), 6, 4, shock_wave_effect_sheet_cnt, effect_alpha, left);
			}
		}
	}

	//�~�j�S�[�����̔j��
	if (mini_golem_break_effect_cnt != 0)
	{
		//�V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_mini_golem_break_effect);

		// �R���C�_�[�̈ʒu�̎擾�i�v���C���[�̈ʒu�j
		b2Vec2 break_pos = mini_golem_delete_effect_position;

		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float break_draw_x = ((break_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float break_draw_y = ((break_pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;




		DrawDividedSpriteBoss(XMFLOAT2(break_draw_x, break_draw_y), 0.0f, XMFLOAT2(GetMiniGolemDrawSize().x * scale * 1.3 * 1.5, GetMiniGolemDrawSize().y * scale * 1.7 * 1.5), 4, 2, mini_golem_break_effect_cnt / 4, effect_alpha, 1);
	}

	////�s���s���̕\��
	//if (panic_effect_sheet_cnt != 0)
	//{
	//	//�V�F�[�_���\�[�X��ݒ�
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_boss_panic_effect);

	//	b2Vec2 panic_pos = m_body->GetPosition();

	//	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	//	float panic_draw_x = ((panic_pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	//	float panic_draw_y = ((panic_pos.y - PlayerPosition::GetPlayerPosition().y-((reality_boss_size.y/2)/BOX2D_SCALE_MANAGEMENT)) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;




	//	DrawDividedSpriteBoss(XMFLOAT2(panic_draw_x, panic_draw_y), 0.0f, XMFLOAT2(panic_effect_size.x * scale , panic_effect_size.y * scale ), 10, 13, panic_effect_sheet_cnt / 4, effect_alpha, 1);
	//}
}

void Boss_1_1::Finalize()
{

	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetBossBody() != nullptr)
	{
		world->DestroyBody(GetBossBody());
		SetBossBody(nullptr);
	}


	if (GetAttackBody() != nullptr)
	{
		world->DestroyBody(GetAttackBody());
		SetAttackBody(nullptr);
	}
}

b2Body* Boss_1_1::GetOutSideBody(void)
{
	return outside_boss_body;
}