//----------------------------------------------------------------------------------------------------
// #name�@UI_Block.cpp
// #description �Z���T�[�ɐڐG������v���C���[�̉E���UI��\������
// #make 2025/02/09
// #update 2025/02/09
// #comment �ǉ��E�C���\��  UI�̎�ނ����񂪂񑫂�����
//         
//          
//----------------------------------------------------------------------------------------------------

#include"UI_Block.h"
#include"sprite.h"
#include"texture.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"
#include"player_position.h"
#include"create_filter.h"
#include"collider_type.h"
#include"game.h"
#include"player.h"



static ID3D11ShaderResourceView* g_Texture = NULL;//�Z���T�[�̃e�N�X�`��
static ID3D11ShaderResourceView* g_UI_texture = NULL;//UI�̃e�N�X�`��
static ID3D11ShaderResourceView* g_arrow_Texture = NULL;//���̃e�N�X�`��


static ID3D11ShaderResourceView* g_move_signboard = NULL;//�Ŕ@����
static ID3D11ShaderResourceView* g_jump_signboard = NULL;//�Ŕ@�W�����v
static ID3D11ShaderResourceView* g_anachor_signboard = NULL;//�Ŕ@�A���J�[

//�Z���T�[�ɐG�ꂽ��UI��\������
UI_block::UI_block(b2Vec2 Position, b2Vec2 block_size, b2Vec2 Sensor_size, b2Vec2 Sensor_Position, Ui_Block_Type type, float texture_angle)
{

	//�T�C�Y���Z�b�g
	SetSize(block_size);
	SetSensorSize(Sensor_size);

	b2Vec2 size;

	size.x = Sensor_size.x / BOX2D_SCALE_MANAGEMENT;
	size.y = Sensor_size.y / BOX2D_SCALE_MANAGEMENT;






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
	block_fixture.filter = createFilterExclude("object_filter", {});

	b2Fixture* object_fixture = m_body->CreateFixture(&block_fixture);

	// �J�X�^���f�[�^���쐬���Đݒ�
	ObjectData* object_data = new ObjectData{ collider_UI_block };
	object_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(object_data);




	int ID = object_data->GenerateID();
	object_data->id = ID;

	SetID(ID);

	
	m_ui_type = type;
	angle = texture_angle * M_PI / 180.0;

};

UI_block::~UI_block()
{
}


void UI_block::Initialize()
{
	if (g_Texture == NULL)
	{
		g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sensor.png");//�Z���T�[�̃e�N�X�`��
		g_arrow_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_arrow.png");//�{�f�B�̃e�N�X�`��
		g_UI_texture = InitTexture(L"asset\\texture\\sample_texture\\Sample_ui_a_bottom.png");


		g_move_signboard = InitTexture(L"asset\\texture\\sample_texture\\signboard_move.png");
		g_jump_signboard = InitTexture(L"asset\\texture\\sample_texture\\signboard_jump.png");
		g_anachor_signboard = InitTexture(L"asset\\texture\\sample_texture\\signboard_move.png");
	}

	/*switch (m_ui_type)
	{
	case VIDEO_BUTTON_A:
		m_video.Initialize("asset/movie/A.mp4", true);
		m_is_video = true;
		m_flag = true;
		break;
	case VIDEO_BUTTON_LEFT_STICK:
		m_video.Initialize("asset/movie/LeftStick.mp4", true);
		m_is_video = true;
		m_flag = true;
		break;
	case VIDEO_BUTTON_RIGHT_STICK:
		m_video.Initialize("asset/movie/RightStick.mp4", true);
		m_is_video = true;
		m_flag = true;
		break;
	case VIDEO_BUTTON_ZR:
		m_video.Initialize("asset/movie/ZR.mp4", true);
		m_is_video = true;
		m_flag = true;
		break;
	default:
		m_is_video = false;
		break;
	}

	m_video.SetState(Video_Pause);
}*/
}
void UI_block::Update()
{

	if (m_body != nullptr)
	{
		if (m_flag == true)
		{
			sheet_cnt += 0.3;
		}

	}
}



void UI_block::Draw()
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
		{ GetSensorSize().x * scale,GetSensorSize().y * scale }
	);

	


	if (m_flag)
	{
		Player& player = Player::GetInstance();
		b2Vec2 player_Pos = player.GetOutSidePlayerBody()->GetPosition();



		
		// �v���C���[�ʒu���l�����ăX�N���[���␳��������
		//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
		float UI_draw_x = ((player_Pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
		float UI_draw_y = ((player_Pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;


		switch (m_ui_type)
		{
		case NULL_UI_TYPE:
			break;
		case ARROW:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_arrow_Texture);

			//draw
			DrawSprite(
				{ draw_x,
				  draw_y },
				angle,
				{ GetSize().x * scale,GetSize().y * scale }
			);
			break;
		case BUTTON_A:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_UI_texture);
			DrawSprite(
				{ UI_draw_x + 50,
				  UI_draw_x - 50 },
				GetBody()->GetAngle(),
				{ 100,100 }///�T�C�Y���擾���邷�ׂ��Ȃ��@�t�B�N�X�`���̃|�C���^�[�ɒǉ����悤���ȁH���ă��x��
			);
			break;
		case BUTTON_B:
			break;

		case VIDEO_BUTTON_A:
		case VIDEO_BUTTON_LEFT_STICK:
		case VIDEO_BUTTON_RIGHT_STICK:
		case VIDEO_BUTTON_ZR:
		/*	m_video.Draw({draw_x,draw_y}, m_size.y);*/
			break;
		default:
			break;

		case MOVE_SIGNBOARD:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_move_signboard);

			DrawSplittingSprite(
				{ UI_draw_x + 50,
				  UI_draw_x - 50 },
				GetBody()->GetAngle(),
				{ GetSize().x * scale,GetSize().y * scale },
				15, 6, sheet_cnt,1.0f
				);

			break;
		}

	
	}



}

void UI_block::Finalize()
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

	if (g_Texture) UnInitTexture(g_Texture);
	if (g_UI_texture) UnInitTexture(g_UI_texture);
	if (g_arrow_Texture) UnInitTexture(g_arrow_Texture);

	m_video.Finalize();

}