//-----------------------------------------------------------------------------------------------------
// #name enemy_floating.cpp
// #description ���V�G
// #make 2025/1/17�@���j�S
// #update 2025/1/17 
// #comment �ǉ��E�C���\��
// 
// 
//----------------------------------------------------------------------------------------------------
#include "enemy_floating.h"
#include "sprite.h"
#include "texture.h"
#include "collider_type.h"
#include "player_position.h"
#include "tool.h"
#include "create_filter.h"


static ID3D11ShaderResourceView* g_EnemyFloating_Texture = NULL;	//���I�G�l�~�[�̈ړ����̃e�N�X�`��
static ID3D11ShaderResourceView* g_EnemyFloating_Explode_Texture = NULL;	//���I�G�l�~�[�̔����̃e�N�X�`��
static ID3D11ShaderResourceView* g_EnemyFloating_Die_Texture = NULL;	//���I�G�l�~�[�̎��ʊԍۂ̃e�N�X�`��

EnemyFloating::EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle)
	:Enemy(ENEMY_FLOATING_LIFE, ENEMY_FLOATING_DAMAGE, ENEMY_FLOATING_SPIRIT_TYPE, ENEMY_FLOATING_SCORE, true, false)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;							//�ÓI�ȃI�u�W�F�N�g�ɂ���Ȃ�ture
	body.position.Set(position.x, position.y);			//�|�W�V�������Z�b�g
	body.angle = angle;									//�p�x�̒�`
	body.userData.pointer = (uintptr_t)this;			//userData�̃|�C���^���` 
	body.fixedRotation = true;							//��]���Œ肷��A�@������I���ɂ���Ɖ�]���Ȃ�
	body.gravityScale = 0.0f;


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
	fixture.density = 1.0f;    //���x
	fixture.friction = 0.05f;  //���C
	fixture.restitution = 0.0f;//�����W��
	fixture.isSensor = true;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����
	fixture.filter = createFilterExclude("enemy_filter", {});

	b2Fixture* enemy_floating_fixture = GetBody()->CreateFixture(&fixture);//Body���Ƀt�B�N�X�`����o�^����


//�v���C���[�����m����Z���T�[
//--------------------------------------------
	b2PolygonShape sensor_shape;                         //shape�ɂ͐F�X�Ȍ^������@�T�[�N���Ƃ��������
	float one_square = 1.0f / BOX2D_SCALE_MANAGEMENT;	//�@1�}�X�̃T�C�Y
	sensor_shape.SetAsBox( size.x * 0.5f + (one_square * 15), size.y * 0.5f + (one_square * 15) );//�����蔻���o�^����4�_�@+((one_square * 7))f�͕������v14�}�X���������邽��

	b2FixtureDef sensor_fixture;
	sensor_fixture.shape = &sensor_shape;    //�V���[�v���t�B�N�X�`���ɓo�^����
	sensor_fixture.density = 1.0f;    //���x
	sensor_fixture.friction = 0.0f;  //���C
	sensor_fixture.restitution = 0.0f;//�����W��
	sensor_fixture.isSensor = true;  //�Z���T�[���ǂ����Atrue�Ȃ炠���蔻��͏�����

	b2Fixture* enemy_sensor_fixture = GetBody()->CreateFixture(&sensor_fixture);//Body���Ƀt�B�N�X�`����o�^����


	// �J�X�^���f�[�^���쐬���Đݒ�
	// ���I�G�l�~�[�ɒl��o�^
	// ���I�G�l�~�[�Ƀ��[�U�[�f�[�^��o�^
	ObjectData* data = new ObjectData{ collider_enemy_floating };
	ObjectData* sensor_data = new ObjectData{ collider_enemy_floating_sensor };
	enemy_floating_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
	enemy_sensor_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(sensor_data);
	data->object_name = Object_Enemy_Floating;
	sensor_data->object_name = Object_Enemy_Floating;
	int ID = data->GenerateID();
	data->id = ID;
	sensor_data->id = ID;
	SetID(ID);

	SetState(ENEMY_FLOATING_STATE_IDLE);
}

void EnemyFloating::Initialize()
{
	if (g_EnemyFloating_Texture == NULL)
	{
		g_EnemyFloating_Texture = InitTexture(L"asset\\texture\\enemy_texture\\enemy_floating_moving.png");//���I�G�l�~�[�̈ړ��̃e�N�X�`��
		g_EnemyFloating_Explode_Texture = InitTexture(L"asset\\texture\\boss_1_1\\mini_golem_break_effect.png");//���I�G�l�~�[�̔����̃e�N�X�`��
		g_EnemyFloating_Die_Texture = InitTexture(L"asset\\texture\\enemy_texture\\enemy_floating_dead.png");//���I�G�l�~�[�����ʃe�N�X�`��
	}
}

void EnemyFloating::Finalize()
{

	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	if (GetBody() != nullptr)
	{
		world->DestroyBody(GetBody());
	}

	if (g_EnemyFloating_Texture) UnInitTexture(g_EnemyFloating_Texture);
	if (g_EnemyFloating_Explode_Texture) UnInitTexture(g_EnemyFloating_Explode_Texture);
	if (g_EnemyFloating_Die_Texture) UnInitTexture(g_EnemyFloating_Die_Texture);

}

void EnemyFloating::Update()
{
	if (GetUse() && m_sensed_player || m_state == ENEMY_FLOATING_STATE_EXPLODE)
	{
		switch (GetState())
		{
		case ENEMY_FLOATING_STATE_IDLE:
			break;
		case ENEMY_FLOATING_STATE_MOVE:
			Move();
			break;
		case ENEMY_FLOATING_STATE_EXPLODE:
			if (m_anim_id >= 7)
			{
				//�\�E���𗎂Ƃ�
				ItemManager& item_manager = ItemManager::GetInstance();
				item_manager.AddSpirit(GetBody()->GetPosition(), { 2.0f,3.0f }, 0.0f, GetSpiritType(), false);

				//���[���h�ɓo�^����body�̍폜
				Box2dWorld& box2d_world = Box2dWorld::GetInstance();
				b2World* world = box2d_world.GetBox2dWorldPointer();
				world->DestroyBody(GetBody());
				SetBody(nullptr);

				//�I�u�W�F�N�g�}�l�[�W���[���̃G�l�~�[�폜
				ObjectManager& object_manager = ObjectManager::GetInstance();
				object_manager.DestroyEnemyFloating(GetID());	
			}
			break;
		case ENEMY_FLOATING_STATE_DIE:
			break;
		default:
			break;
		}

	}
	else if (!GetUse())
	{

		//�G�l�~�[���u���񂾎��̃T�E���h
		app_atomex_start(Enemy_Knock_Down2_Sound);

		//�\�E���𗎂Ƃ�
		ItemManager& item_manager = ItemManager::GetInstance();
		item_manager.AddSpirit(GetBody()->GetPosition(), { 2.0f,3.0f }, 0.0f, GetSpiritType(), false);


		b2Vec2 now_positon = GetBody()->GetPosition();
		b2Vec2 now_size = GetSize();
		b2Vec2 now_vec = GetRandomVelocity(47.0f);
		while (now_vec.x < 1.2f && now_vec.x >-1.2f)
		{
			now_vec = GetRandomVelocity(47.0f);
		}

		item_manager.AddDamageValue(b2Vec2{ now_positon.x, now_positon.y - 0.2f }, b2Vec2{ 1.0f,1.0f }, 0.0f, DamageOwnerType_enemy, ENEMY_FLOATING_LIFE);

		//���[���h�ɓo�^����body�̍폜
		Box2dWorld& box2d_world = Box2dWorld::GetInstance();
		b2World* world = box2d_world.GetBox2dWorldPointer();
		world->DestroyBody(GetBody());
		SetBody(nullptr);

		//�I�u�W�F�N�g�}�l�[�W���[���̃G�l�~�[�폜
		ObjectManager& object_manager = ObjectManager::GetInstance();
		object_manager.DestroyEnemyFloating(GetID());

		
		object_manager.AddChangeEnemyFilterAndBody(now_positon, b2Vec2(now_size.x * 2, now_size.y * 2), b2Vec2_zero, g_EnemyFloating_Die_Texture, 4, 4, now_vec);

	}
}

void EnemyFloating::Draw()
{
	if (GetBody() != nullptr)
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
		case ENEMY_FLOATING_STATE_IDLE:
			//�\��e�N�X�`�����w��
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Texture);

			DrawSplittingSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale },
				5, 5, m_anim_id, 3.0f
			);
			m_anim_id++;
			m_anim_id = (int)m_anim_id % 25;
			break;
		case ENEMY_FLOATING_STATE_MOVE:
			//�\��e�N�X�`�����w��
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Texture);

			DrawSplittingSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale },
				5, 5, m_anim_id, 3.0f
			);
			m_anim_id++;
			m_anim_id = (int)m_anim_id % 25;
			break;
		case ENEMY_FLOATING_STATE_EXPLODE:
			//�\��e�N�X�`�����w��
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Explode_Texture);
			DrawSplittingSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale * 2.0f ,GetSize().y * scale * 2.0f },
				4, 2, m_anim_id, 3.0f
			);
			m_anim_id+=0.2f;
			break;
		case ENEMY_FLOATING_STATE_DIE:
			//�\��e�N�X�`�����w��
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_EnemyFloating_Die_Texture);

			DrawSplittingSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale },
				5, 5, m_anim_id, 3.0f
			);
			m_anim_id++;
			m_anim_id = (int)m_anim_id % 25;
			break;
		default:
			break;
		}

	}

}

void EnemyFloating::SetState(ENEMY_FLOATING_STATE state)
{
	if (m_state == ENEMY_FLOATING_STATE_EXPLODE)
	{
		return;
	}
	switch (state)
	{
	case ENEMY_FLOATING_STATE_IDLE:
		break;
	case ENEMY_FLOATING_STATE_MOVE:
		break;
	case ENEMY_FLOATING_STATE_EXPLODE:
		app_atomex_start(Enemy_MiniGolem_Explosion_Sound);//���V�G�̔j��
		// �t�B���^�[��ύX
		updateFixtureFilter("enemy_filter", { "Player_filter" });
		m_anim_id = 0.0f;
		break;
	case ENEMY_FLOATING_STATE_DIE:
		m_anim_id = 0.0f;
		break;
	}
	m_state = state;
}

void EnemyFloating::updateFixtureFilter(const std::string& category, const std::vector<std::string>& includeMasks)
{
	// �{�f�B�̍ŏ��̃t�B�N�X�`�����擾
	b2Fixture* fixture = GetBody()->GetFixtureList();

	// �t�B�N�X�`�������݂��Ȃ��ꍇ�͑������^�[��
	if (!fixture)
	{
		return;
	}

	// �V�����t�B���^�[���쐬
	b2Filter newFilter = createFilterExclude(category, includeMasks);

	// ���ׂẴt�B�N�X�`���ɑ΂��ăt�B���^�[���X�V
	while (fixture)
	{


		fixture->SetFilterData(newFilter);
		fixture = fixture->GetNext(); // ���̃t�B�N�X�`���Ɉړ�
	}
}


//�ړ�
void EnemyFloating::Move()
{
	if (GetBody() != nullptr)
	{
		//�v���C���[�Ǐ](�Ȉ�)
		//�v���C���[�̃|�W�V�����擾
		b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

		//  �v���C���[�֌������x�N�g��
		b2Vec2 vec;
		vec.x = player_position.x - GetBody()->GetPosition().x;
		vec.y = player_position.y - GetBody()->GetPosition().y;



		GetBody()->SetLinearVelocity(b2Vec2(vec.x * m_speed, vec.y * m_speed));

	}

}
