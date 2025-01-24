//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description ���I�G�l�~�[(�v���C���[�Ǐ])��cpp�t�@�C��
// #make 2024/11/20
// #update 2024/12/20
// #comment �ǉ��E�C���\��
//          �E�X�e�[�^�X����
//           
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy_dynamic.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_position.h"
#include"player_stamina.h"
#include"contactlist.h"
#include"anchor_spirit.h"
#include"Item_Manager.h"
#include"create_filter.h"


static ID3D11ShaderResourceView* g_EnemyDynamic_Texture;//���I�G�l�~�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_EnemySensor_Texture = NULL;	//�G�l�~�[�̃Z���T�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_EnemySensor2_Texture = NULL;	//�G�l�~�[�̃Z���T�[�̃e�N�X�`��

EnemyDynamic::EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_DYNAMIC_LIFE, ENEMY_DYNAMIC_DAMAGE, ENEMY_DYNAMIC_SOULGAGE, ENEMY_DYNAMIC_SCORE, true, false)
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

	b2CircleShape shape2;
	shape2.m_radius = size.x / 2;

	b2FixtureDef fixture2;
	fixture2.shape = &shape2;
	fixture2.density = 1.0f;
	fixture2.friction = 0.001f;//���C
	fixture2.restitution = 0.0f;//�����W��
	fixture2.isSensor = false;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	fixture2.filter=createFilterExclude("enemy_filter", {});

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
	//----------------------------------------------------------------------------------------------------
	b2Vec2 size_sensor2;
	size_sensor2.x = body_size.x / BOX2D_SCALE_MANAGEMENT * (2);
	size_sensor2.y = body_size.y / BOX2D_SCALE_MANAGEMENT;

	b2PolygonShape shape_sensor2;
	shape_sensor2.SetAsBox(size_sensor2.x * 0.5, size_sensor2.y * 0.5);
	m_size_sensor_2 = b2Vec2(body_size.x * 2, body_size.y);

	b2FixtureDef fixture_sensor2;
	fixture_sensor2.shape = &shape_sensor2;
	fixture_sensor2.density = 0.0f;//���x
	fixture_sensor2.friction = 0.0f;//���C
	fixture_sensor2.restitution = 0.0f;//�����W��
	fixture_sensor2.isSensor = true;//�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	//====================================================================================================

	//Body�Ƀt�B�N�X�`����o�^����
	b2Fixture* enemy_static_fixture = GetBody()->CreateFixture(&fixture2);
	b2Fixture* enemy_sensor_fixture = GetBody()->CreateFixture(&fixture_sensor);
	b2Fixture* enemy_sensor_fixture2 = GetBody()->CreateFixture(&fixture_sensor2);

	// �J�X�^���f�[�^���쐬���Đݒ�
	// ���I�G�l�~�[�ɒl��o�^
	// ���I�G�l�~�[�Ƀ��[�U�[�f�[�^��o�^
	ObjectData* data = new ObjectData{ collider_enemy_dynamic };
	enemy_static_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
	ObjectData* sensor_data = new ObjectData{ collider_enemy_sensor };
	enemy_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(sensor_data);
	ObjectData* sensor_data2 = new ObjectData{ collider_enemy_sensor_move };
	enemy_sensor_fixture2->GetUserData().pointer = reinterpret_cast<uintptr_t>(sensor_data2);

	//ID�o�^(�Z���T�[�̃f�[�^�ɂ��G�l�~�[�Ɠ���ID������)
	data->object_name = Object_Enemy_Dynamic;
	int ID = data->GenerateID();
	data->id = ID;
	sensor_data->id = ID;
	sensor_data2->id = ID;
	SetID(ID);

	m_state = ENEMY_STATE_NULL;
	m_move_force = body_size.x * body_size.y;
	m_sensor_move_size = (2 * body_size.x);
}

void EnemyDynamic::Initialize()
{
	g_EnemyDynamic_Texture = InitTexture(L"asset\\texture\\sample_texture\\enemy_1.png");//���I�G�l�~�[�̃e�N�X�`��
	g_EnemySensor_Texture = InitTexture(L"asset\\texture\\sample_texture\\xxx_enemy_sensor.png");//�G�l�~�[�̃Z���T�[�̃e�N�X�`��
	g_EnemySensor2_Texture = InitTexture(L"asset\\texture\\sample_texture\\xxx_enemy_sensor_left.png");//�G�l�~�[�̃Z���T�[�̃e�N�X�`��
}

void EnemyDynamic::Finalize()
{
	UnInitTexture(g_EnemyDynamic_Texture);
	UnInitTexture(g_EnemySensor_Texture);
	UnInitTexture(g_EnemySensor2_Texture);

	//���[���h�ɓo�^����body�̍폜
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();
	world->DestroyBody(GetBody());
	SetBody(nullptr);
}

void EnemyDynamic::Update()
{
	if (GetUse())
	{
		switch (GetState())
		{
		case ENEMY_STATE_MOVE:
			Move();
			m_old_state = ENEMY_STATE_MOVE;
			break;
		case ENEMY_STATE_ATTACK:
			Attack();
			m_old_state = ENEMY_STATE_ATTACK;
			break;
		default:
			if (GetInScreen())
			{
				SetState(ENEMY_STATE_MOVE);

				SetDirectionBasedOnPlayer();
			}
			m_old_state = ENEMY_STATE_NULL;
			break;
		}
	}
	else if (!GetUse())
	{
		//�\�E���𗎂Ƃ�
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(GetBody()->GetPosition(), { 1.0f,2.0f }, 0.0f, GetSoulgage());

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

		object_manager.DestroyEnemyDynamic(GetID());
	}
}

void EnemyDynamic::Draw()
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

	//�\��e�N�X�`�����w��
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyDynamic_Texture);

	//draw
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ GetSize().x * scale*2.0f , GetSize().y * scale*2.0f }
	);


	//============================================================
	//�e�X�g:�Z���T�[�`��
	//============================================================
	//�\��e�N�X�`�����w��
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemySensor2_Texture);
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ m_size_sensor_2.x * scale , m_size_sensor_2.y * scale }
	);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemySensor_Texture);
	DrawSprite(
		{ draw_x,
		  draw_y },
		GetBody()->GetAngle(),
		{ m_size_sensor.x * scale , m_size_sensor.y * scale }
	);
}

//�ړ�
void EnemyDynamic::Move()
{
	//�ړ��ʎ擾
	b2Vec2 liner_velocity = GetBody()->GetLinearVelocity();

	//���n����(y���ړ���������Β��n��)
	if (liner_velocity.y != 0.0)
	{
		m_is_ground = false;
	}
	else
	{
		m_is_ground = true;
	}

	//�R�Ŕ��]
	if (m_ground_cnt == m_sensor_move_size && m_old_ground_cnt > m_sensor_move_size && m_is_ground && (GetBody()->GetLinearVelocity() != b2Vec2(0.0,0.0)))
	{
		SetDirection(!GetDirection());
	}

	//��ʓ��Ȃ�ړ�
	if(GetInScreen())
	{
		//�ړ����Ă��Ȃ����A�O��ړ����������ꍇ�W�����v
		if (liner_velocity == b2Vec2(0.0, 0.0) && m_old_state == ENEMY_STATE_MOVE)
		{
			GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.0, m_jump_force * m_move_force), true);
		}
		else
		{
			GetBody()->SetLinearVelocity(b2Vec2(0.0, liner_velocity.y));
			if (GetDirection())
			{
				GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-m_speed * m_move_force, 0.0), true);
			}
			else if(!GetDirection())
			{
				GetBody()->ApplyLinearImpulseToCenter(b2Vec2(m_speed* m_move_force, 0.0), true);
			}
		}
	}
	else
	{
		//��ʊO�ɏo�Ă����ꍇ�A�X�e�[�^�X��ς��ړ��p�֐����Ăяo���Ȃ�
		SetState(ENEMY_STATE_NULL);
	}

	//�ڐG���̒n�ʂ̐����L��
	m_old_ground_cnt = m_ground_cnt;
}

//�U��
void EnemyDynamic::Attack()
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
		GetBody()->SetType(b2_dynamicBody);
		SetState(ENEMY_STATE_NULL);
		return;
	}

}

//�Z���T�[�ƃv���C���[���G�ꂽ���̏���
void EnemyDynamic::CollisionSensorPlayer()
{
	//�G�l�~�[���U�����Ȃ牽�����Ȃ�
	if ((GetState() != ENEMY_STATE_ATTACK) && (m_is_ground))
	{
		SetDirectionBasedOnPlayer();

		//�U����ԂɈڍs
		SetState(ENEMY_STATE_ATTACK);
	}
}

void EnemyDynamic::SetDirectionBasedOnPlayer()
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