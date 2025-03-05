//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description �ÓI�G�l�~�[��cpp�t�@�C��
// #make 2024/11/19
// #update 2025/02/10
// #comment �ǉ��E�C���\��
//          �E
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_static.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"
#include"contactlist.h"
#include"anchor_spirit.h"
#include"object_manager.h"
#include"Item_Manager.h"

static ID3D11ShaderResourceView* g_EnemyStatic_Texture = NULL;	//�ÓI�G�l�~�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_EnemyStatic_Texture_Move;	  //�ÓI�G�l�~�[�̈ړ��e�N�X�`��
static ID3D11ShaderResourceView* g_EnemyStatic_Texture_Attack;	  //�ÓI�G�l�~�[�̍U���e�N�X�`��
static ID3D11ShaderResourceView* g_EnemyStatic_Texture_Destroyed;//�ÓI�G�l�~�[�̎��S�e�N�X�`��
static ID3D11ShaderResourceView* g_EnemySensor_Texture = NULL;	  //�G�l�~�[�̃Z���T�[�̃e�N�X�`��

EnemyStatic::EnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_STATIC_LIFE, ENEMY_STATIC_DAMAGE, ENEMY_STATIC_SPIRIT_TYPE, ENEMY_STATIC_SCORE, true, false)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;							//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�ture
	body.position.Set(position.x, position.y);			//�|�W�V�������Z�b�g
	body.angle = angle;									//�p�x�̒�`
	body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
	body.fixedRotation = true;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�


	Box2dWorld& box2d_world = Box2dWorld::GetInstance();//���[���h�̃C���X�^���X���擾����
	b2World* world = box2d_world.GetBox2dWorldPointer();//���[���h�̃|�C���^�������Ă���

	SetBody(world->CreateBody(&body));//Body�����[���h�ɌŒ�


	SetSize(body_size);//�\���p�ɃT�C�Y���Z�b�g���Ƃ��A�\���̂Ƃ���GetSize���Ăяo��



	b2Vec2 size;
	size.x = body_size.x / BOX2D_SCALE_MANAGEMENT;//�T�C�Y���P�ɂ���Ɓ@1m*1m�ɂȂ邽�߁@�T�C�Y�������āA�������Z�̋����𑀍삵�₷������
	size.y = body_size.y / BOX2D_SCALE_MANAGEMENT;



	b2PolygonShape shape;                         //shape�ɂ͐F�X�Ȍ^������@�T�[�N���Ƃ��������
	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);//�����蔻���o�^����4�_�@*0.5����̂�

	b2FixtureDef fixture;
	fixture.shape = &shape;    //�V���[�v���t�B�N�X�`���ɓo�^����
	fixture.density = 0.1f;    //���x
	fixture.friction = 0.05f;  //���C
	fixture.restitution = 0.0f;//�����W��
	fixture.isSensor = false;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	//====================================================================================================
	//�Z���T�[�̓o�^
	//====================================================================================================
	b2Vec2 size_sensor;
	size_sensor.x = body_size.x / BOX2D_SCALE_MANAGEMENT * 2;
	size_sensor.y = body_size.y / BOX2D_SCALE_MANAGEMENT;

	b2PolygonShape shape_sensor;
	shape_sensor.SetAsBox(size_sensor.x * 0.5, size_sensor.y * 0.5);
	m_size_sensor = b2Vec2(body_size.x * 2, body_size.y);

	b2FixtureDef fixture_sensor;
	fixture_sensor.shape = &shape_sensor;
	fixture_sensor.density = 0.0f;//���x
	fixture_sensor.friction = 0.0f;//���C
	fixture_sensor.restitution = 0.0f;//�����W��
	fixture_sensor.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	//====================================================================================================

	b2Fixture* enemy_static_fixture = GetBody()->CreateFixture(&fixture);//Body���Ƀt�B�N�X�`����o�^����
	b2Fixture* enemy_sensor_fixture = GetBody()->CreateFixture(&fixture_sensor);


	// �J�X�^���f�[�^���쐬���Đݒ�
	// ���I�G�l�~�[�ɒl��o�^
	// ���I�G�l�~�[�Ƀ��[�U�[�f�[�^��o�^
	m_object_data = std::make_unique<ObjectData>(collider_enemy_static);
	enemy_static_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_object_data.get());
	m_sensor_data = std::make_unique<ObjectData>(collider_enemy_static_sensor);
	enemy_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_sensor_data.get());

	m_object_data->object_name = Object_Enemy_Static;
	int ID = m_object_data->GenerateID();
	m_object_data->id = ID;
	m_sensor_data->id = ID;
	SetID(ID);
}

void EnemyStatic::Initialize()
{
	if (g_EnemyStatic_Texture == NULL) {
		g_EnemyStatic_Texture = InitTexture(L"asset\\texture\\sample_texture\\enemy_1.png");//�ÓI�G�l�~�[�̃e�N�X�`��
		g_EnemyStatic_Texture_Move = InitTexture(L"asset\\texture\\enemy_texture\\enemy_move.png");//�ÓI�G�l�~�[�̈ړ��e�N�X�`��
		g_EnemyStatic_Texture_Attack = InitTexture(L"asset\\texture\\enemy_texture\\enemy_attack.png");//�ÓI�G�l�~�[�̍U���e�N�X�`��
		g_EnemyStatic_Texture_Destroyed = InitTexture(L"asset\\texture\\enemy_texture\\enemy_destroyed.png");//�ÓI�G�l�~�[�̎��S�e�N�X�`��
		g_EnemySensor_Texture = InitTexture(L"asset\\texture\\sample_texture\\xxx_enemy_sensor.png");//�G�l�~�[�̃Z���T�[�̃e�N�X�`��
	}
}

void EnemyStatic::Finalize()
{
	if (g_EnemyStatic_Texture) UnInitTexture(g_EnemyStatic_Texture);
	if (g_EnemyStatic_Texture_Move) UnInitTexture(g_EnemyStatic_Texture_Move);
	if (g_EnemyStatic_Texture_Attack) UnInitTexture(g_EnemyStatic_Texture_Attack);
	if (g_EnemyStatic_Texture_Destroyed) UnInitTexture(g_EnemyStatic_Texture_Destroyed);
	if (g_EnemySensor_Texture) UnInitTexture(g_EnemySensor_Texture);


	//���[���h�ɓo�^����body�̍폜
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	world->DestroyBody(GetBody());
	SetBody(nullptr);
}

void EnemyStatic::Update()
{
	if (GetUse())
	{
		switch (GetState())
		{
		case ENEMY_STATIC_STATE_IDLE:
			Idle();
			m_old_state = ENEMY_STATIC_STATE_IDLE;
			break;
		case ENEMY_STATIC_STATE_ATTACK:
			Attack();
			m_old_state = ENEMY_STATIC_STATE_ATTACK;
			break;
		default:
			if (GetInScreen())
			{
				SetState(ENEMY_STATIC_STATE_IDLE);

				SetDirectionBasedOnPlayer();
			}
			m_old_state = ENEMY_STATIC_STATE_NULL;
			break;
		}
	}
	else if (!GetUse())
	{
		//�\�E���𗎂Ƃ�
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(GetBody()->GetPosition(), { 2.0f,3.0f }, 0.0f, GetSpiritType(), false);



		b2Vec2 now_positon = GetBody()->GetPosition();
		b2Vec2 now_size = GetSize();
		b2Vec2 now_vec = GetBody()->GetLinearVelocity();

		item_manager.AddDamageValue(b2Vec2{ now_positon.x, now_positon.y - 0.2f }, b2Vec2{ 1.5f,1.5f }, 0.0f, DamageOwnerType_enemy, ENEMY_STATIC_LIFE);

		//���[���h�ɓo�^����body�̍폜
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());
		SetBody(nullptr);

		//�I�u�W�F�N�g�}�l�[�W���[���̃G�l�~�[�폜
		ObjectManager& object_manager = ObjectManager::GetInstance();

		if (object_manager.FindEnemyAttackByID(GetID()))
		{
			object_manager.FindEnemyAttackByID(GetID())->SetUse(false);
		}

		object_manager.DestroyEnemyStatic(GetID());

		object_manager.AddChangeEnemyFilterAndBody(now_positon, b2Vec2(now_size.x * 2, now_size.y * 2), b2Vec2_zero, g_EnemyStatic_Texture_Destroyed, 4, 4, now_vec);
	}
}

void EnemyStatic::Draw()
{
	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	b2Vec2 position = GetBody()->GetPosition();

	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

	switch (GetState())
	{
	case ENEMY_STATIC_STATE_IDLE:
		//�\��e�N�X�`�����w��
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyStatic_Texture_Move);

		DrawDividedSpritePlayer(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale * 2.0f ,GetSize().y * scale * 2.0f },
			5, 5, m_anim_id, 3.0, m_direction
		);
		m_anim_id++;
		m_anim_id = m_anim_id % 25;
		break;
	case ENEMY_STATIC_STATE_ATTACK:
		//�\��e�N�X�`�����w��
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyStatic_Texture_Attack);

		//draw
		DrawDividedSpritePlayer(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale * 3.0f ,GetSize().y * scale * 2.0f },
			5, 5, m_anim_id, 3.0, m_direction
		);
		if (m_attack_counter % 2 == 0)
		{
			m_anim_id++;
		}
		m_anim_id = m_anim_id % 25;
		break;
	default:
		//�\��e�N�X�`�����w��
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyStatic_Texture_Move);

		DrawDividedSpritePlayer(
			{ draw_x,
			  draw_y },
			GetBody()->GetAngle(),
			{ GetSize().x * scale * 2.0f ,GetSize().y * scale * 2.0f },
			5, 5, 0, 3.0, m_direction
		);
		break;
	}


	//============================================================
	//�e�X�g:�Z���T�[�`��
	//============================================================
	/*GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemySensor_Texture);
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ m_size_sensor.x * scale , m_size_sensor.y * scale }
	);*/
}

void EnemyStatic::Idle()
{
	SetDirectionBasedOnPlayer();
	//�U���N�[���^�C��
	m_attack_cooltime_counter++;
}

//�U��
void EnemyStatic::Attack()
{
	//�t���[���J�E���g
	m_attack_counter++;
	//�U�����͒�~
	GetBody()->SetType(b2_staticBody);

	//�U�������t���[���ɂȂ�����U���𐶐�
	if (m_attack_counter == m_attack_birth)
	{
		ObjectManager& object_manager = ObjectManager::GetInstance();
		b2Vec2 pos = GetBody()->GetPosition();
		if (GetDirection())
		{
			object_manager.AddEnemyAttack(b2Vec2(pos.x - GetSize().x / 4, pos.y), GetSize(), 0.0f, GetID());
		}
		else
		{
			object_manager.AddEnemyAttack(b2Vec2(pos.x + GetSize().x / 4, pos.y), GetSize(), 0.0f, GetID());
		}
	}

	//�U�����삪�S�ďI��������A�����悤�ɖ߂��Ĉړ���Ԃɂ���
	if (m_attack_counter > m_attack_finish)
	{
		m_attack_counter = 0;
		SetState(ENEMY_STATIC_STATE_NULL);
		m_anim_id = 0;
		m_attack_cooltime_counter = 0;
		return;
	}
}

//�Z���T�[�ƃv���C���[���G�ꂽ���̏���
void EnemyStatic::CollisionSensorPlayer()
{
	//�G�l�~�[���U�����Ȃ牽�����Ȃ�
	if ((GetState() != ENEMY_STATE_ATTACK) && m_attack_cooltime_counter >= m_attack_cooltime)
	{
		SetDirectionBasedOnPlayer();
		m_anim_id = 0;
		//�U����ԂɈڍs
		SetState(ENEMY_STATIC_STATE_ATTACK);
	}
}

void EnemyStatic::SetDirectionBasedOnPlayer()
{
	//�v���C���[�Ǐ](�Ȉ�)
		//�v���C���[�̃|�W�V�����擾
	b2Vec2 player_position;
	player_position.x = PlayerPosition::GetPlayerPosition().x;
	player_position.y = PlayerPosition::GetPlayerPosition().y;

	//�ړ�����
	b2Vec2 enemy_vector;
	enemy_vector.x = player_position.x - GetBody()->GetPosition().x;
	enemy_vector.y = player_position.y - GetBody()->GetPosition().y;

	bool old_direction = GetDirection();

	//�v���C���[�̕���������
	if (enemy_vector.x > 0)
	{
		SetDirection(false);
	}
	else
	{
		SetDirection(true);
	}
}
