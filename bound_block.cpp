//-----------------------------------------------------------------------------------------------------
// #name bound_block.cpp
// #description �{�X��Őݒu����o�E���h����I�u�W�F�N�g
// #make 2025/02/06�@�i��`��
// #update 2025/02/06
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------

#include"bound_block.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"collider_type.h"
#include"player_position.h"
#include"1-1_boss.h"
#include"create_filter.h"
#include"tool.h"
#include"player.h"
#include"keyboard.h"
#include"Xinput_controller.h"
#include"scene.h"

static ID3D11ShaderResourceView* g_bound_block_texture1_open = NULL;//�o�E���h�u���b�N�̃e�N�X�`�����F
static ID3D11ShaderResourceView* g_bound_block_texture1_close = NULL;//�o�E���h�u���b�N�̃e�N�X�`�����F

static ID3D11ShaderResourceView* g_bound_block_texture2_open = NULL;//�o�E���h�u���b�N�̃e�N�X�`���D�F
static ID3D11ShaderResourceView* g_bound_block_texture2_close = NULL;//�o�E���h�u���b�N�̃e�N�X�`���D�F

boss_bound_block::boss_bound_block(b2Vec2 position, b2Vec2 size, b2Vec2 vec, Boss_Room_Level level,int texture_type)
{
	SetSize(size);//�`��p�̃T�C�Y��ۑ�

	//���[���h�̃C���X�^���X�������Ă���
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	b2World* world = box2d_world.GetBox2dWorldPointer();


	//�T�C�Y�𒲐�����
	b2Vec2 body_size;
	body_size.x = size.x / BOX2D_SCALE_MANAGEMENT;
	body_size.y = size.y / BOX2D_SCALE_MANAGEMENT;

	//�{�f�B���쐬����
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(position.x, position.y);
	body.fixedRotation = false;

	b2Body* m_Body = world->CreateBody(&body);

	SetBody(m_Body);


	//�`�̒�`
	b2PolygonShape shape;
	shape.SetAsBox(body_size.x * 0.5, body_size.y * 0.5);


	//-----------------------------------------------------
	//	fixture�����
	b2FixtureDef fixture;

	SceneManager& sceneManager = SceneManager::GetInstance();
	fixture.shape = &shape;
	fixture.density = 1.0f;
	if (sceneManager.GetStageName() == STAGE_BOSS)
	{
		fixture.friction = 0.01f;
	}
	else
	{
		fixture.friction = 0.3f;
	}
	fixture.restitution = 0.3f;
	fixture.isSensor = false;
	fixture.filter = createFilterExclude("ground_filter", {});

	b2Fixture* m_fixture = m_Body->CreateFixture(&fixture);

	// �V�����t�B���^�[���쐬


	//�J�X�^���f�[�^���쐬
// ���j�[�N�|�C���^�[���g���� ObjectData ���쐬
	m_objectData = std::make_unique<ObjectData>(collider_bound_block);
	m_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_objectData.get());

	int ID = m_objectData->GenerateID();
	m_objectData->id = ID;
	m_objectData->object_name = NULL_object;
	SetID(ID);


	//�W�����v�ʂ�n��
	vectol = vec;

	BossRoomLevel = level;

	Texture_type = texture_type;


}

boss_bound_block::~boss_bound_block()
{
	Finalize();
}



void boss_bound_block::Initialize()
{
	if (g_bound_block_texture1_open == NULL) {
		g_bound_block_texture1_open = InitTexture(L"asset\\texture\\stage_1_1_object\\bound_block_open_type1.png");//�o�E���h�u���b�N�̃e�N�X�`��
		g_bound_block_texture1_close = InitTexture(L"asset\\texture\\stage_1_1_object\\bound_block_close_type1.png");//�o�E���h�u���b�N�̃e�N�X�`��
		g_bound_block_texture2_open = InitTexture(L"asset\\texture\\stage_1_1_object\\bound_block_open_type2.png");//�o�E���h�u���b�N�̃e�N�X�`��
		g_bound_block_texture2_close = InitTexture(L"asset\\texture\\stage_1_1_object\\bound_block_close_type2.png");//�o�E���h�u���b�N�̃e�N�X�`��
	}
}

void boss_bound_block::Update()
{
	if (isUse)
	{
		Boss_1_1& boss = Boss_1_1::GetInstance();
		if (boss.GetBossFieldLevel() > BossRoomLevel && break_flag == false)
		{
			m_body->SetType(b2_dynamicBody);

			// �t�B���^�[��ύX
			b2Fixture* fixture = m_body->GetFixtureList();

			// �t�B�N�X�`�������݂��Ȃ��ꍇ�͑������^�[��
			if (!fixture) {
				return;
			}

			// �V�����t�B���^�[���쐬
			b2Filter newFilter = createFilterExclude("Boss_field_filter", { "ground_filter","object_filter","No_Entry_block_body_filter" });
			fixture->SetFilterData(newFilter);

			break_flag = true;

			int x = GetRandomInt(1, 10);


			int minus = GetRandomInt(0, 1);

			if (minus == 1)
			{
				minus = -1;
			}
			else
			{
				minus = 1;
			}

			m_body->ApplyLinearImpulseToCenter(b2Vec2(x / 5 * minus, 0.0f), true);

		}

		if (break_flag == true)
		{
			body_delete_cnt++;
		}

		//�R�b�Ԃ�������{�f�B������
		if (body_delete_cnt > 180)
		{
			Box2dWorld& box2d_world = Box2dWorld::GetInstance();
			b2World* world = box2d_world.GetBox2dWorldPointer();

			if (m_body != nullptr)
			{
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
				world->DestroyBody(m_body);
				break_flag = false;
				isUse = false;
				SetBody(nullptr);
			}

		}
		Player_jump();
	}



}

void boss_bound_block::Draw()
{


	if (m_body != nullptr)
	{
		// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
		float scale = SCREEN_SCALE;

		// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
		b2Vec2 screen_center;
		screen_center.x = SCREEN_CENTER_X;
		screen_center.y = SCREEN_CENTER_Y;
		if (Change_Texture_Size_Frame == 0)
		{
		

			b2Vec2 Pos = GetBody()->GetPosition();


			float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

			switch (Texture_type)
			{
			case 0:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_bound_block_texture1_close);
				break;
			case 1:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_bound_block_texture2_close);
			default:
				break;
			}
		


			//draw
			DrawSprite(
				{ draw_x,
				  draw_y-10 },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale*1.3f }
			);
		}
		else
		{
		

			b2Vec2 Pos = GetBody()->GetPosition();

			


			float draw_x = ((Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
			float draw_y = ((Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;
			switch (Texture_type)
			{
			case 0:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_bound_block_texture1_open);
				break;
			case 1:
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_bound_block_texture2_open);
			default:
				break;
			}
			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				GetBody()->GetAngle(),
				{ GetSize().x * scale ,GetSize().y * scale *2}
			);

			Change_Texture_Size_Frame++;

			if (30 < Change_Texture_Size_Frame)
			{
				Change_Texture_Size_Frame = 0;
			}
		}
	}

}
void boss_bound_block::Player_jump()
{
	if (jump_flag == true)
	{
		// �R���g���[���[�̓��͂̎󂯎��
		ControllerState state = GetControllerInput();
		if ((Keyboard_IsKeyDown(KK_UP) || (state.buttonA)))
		{
			Player& player = Player::GetInstance();//�V���O���g��

			b2Body* player_body = player.GetOutSidePlayerBody();

			player_body->ApplyLinearImpulseToCenter(vectol, true);




			//�`�F���W�e�N�X�`��
			Change_Texture_Size_Frame = 1;

			jump_flag = false;

		}
	}
}



void boss_bound_block::Finalize()
{
	
	if (g_bound_block_texture1_open) UnInitTexture(g_bound_block_texture1_open);
	if (g_bound_block_texture1_close) UnInitTexture(g_bound_block_texture1_close);

	if (g_bound_block_texture2_open) UnInitTexture(g_bound_block_texture2_open);
	if (g_bound_block_texture2_close) UnInitTexture(g_bound_block_texture2_close);


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