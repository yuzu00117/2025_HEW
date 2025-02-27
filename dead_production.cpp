//-----------------------------------------------------------------------------------------------------
// #name dead_production.cpp
// #description    ���S���o
// #make 2025/02/08�@�i��`��
// #update 2025/02/08
// #comment �ǉ��E�C���\��
//
//-------------------------------------------------------------------------------------------------------
#include"dead_production.h"
#include"texture.h"
#include"sprite.h"
#include"easing.h"
#include"player_life.h"
#include"player_stamina.h"
#include"hit_stop.h"
#include"player_position.h"
#include"world_box2d.h"
#include"player.h"
#include"player_life.h"
#include"sound.h"

static ID3D11ShaderResourceView* g_Black_texture = NULL;//���̃e�N�X�`��
static ID3D11ShaderResourceView* g_Black_hole_texture = NULL;//�^�񒆂��󂢂Ă鍕�̃e�N�X�`��

static ID3D11ShaderResourceView* g_player_icon_texture = NULL;//�v���C���[�̃A�C�R��

static ID3D11ShaderResourceView* g_batten_texture = NULL;//�^�񒆂̂΂��Ă�

static ID3D11ShaderResourceView* g_number_texture = NULL;//�����̃e�N�X�`


static ID3D11ShaderResourceView* g_game_over_back_ground_texture = NULL;//�w�i
static ID3D11ShaderResourceView* g_game_over_tama_start_texture = NULL;//���܂����@�n�܂�
static ID3D11ShaderResourceView* g_game_over_tama_loop_texture = NULL;//���܂����@���[�v
static ID3D11ShaderResourceView* g_game_over_text_texture = NULL;//�e�L�X�g



float dead_production::Dead_Cnt = 0;
float dead_production::Black_fade_rate = 0;
float dead_production::Black_hole_rate = 0;
bool dead_production::Dead_Flag = false;
float dead_production::Game_over_cnt = 0;

float dead_production::Game_over_Black_fade_rate = 0;
float dead_production::Game_over_tama_cnt = 0;
float dead_production::Game_over_text_cnt = 0;


void dead_production::Initialize()
{
	if (g_Black_texture == NULL)
	{
		g_Black_texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
		g_Black_hole_texture = InitTexture(L"asset\\texture\\sample_texture\\sample_fade_black.png");
		g_player_icon_texture= InitTexture(L"asset\\texture\\UI_soul_gage\\player_icon.png");
		g_batten_texture = InitTexture(L"asset\\texture\\sample_texture\\batten.png");

		g_number_texture= InitTexture(L"asset\\texture\\sample_texture\\sample_number_white.png");

		g_game_over_back_ground_texture = InitTexture(L"asset\\texture\\game_over_texture\\game_over_background.png");
		g_game_over_tama_start_texture	= InitTexture(L"asset\\texture\\game_over_texture\\game_over_tama_start.png");
		g_game_over_tama_loop_texture	= InitTexture(L"asset\\texture\\game_over_texture\\game_over_tama_loop.png");
		g_game_over_text_texture		= InitTexture(L"asset\\texture\\game_over_texture\\game_over_text.png");
	}
}


void dead_production::Reset()
{
	Dead_Cnt = 0;
	Black_fade_rate = 0;
	Black_hole_rate = 0;
	Game_over_cnt = 0;
	Game_over_Black_fade_rate = 0;
	Game_over_tama_cnt = 0;
	Dead_Flag = false;
}

void dead_production::Update()
{
	if (Dead_Cnt == 0)
	{
		Player& player = Player::GetInstance();
		player.SetState(player_dead_state);
		player.SetDrawCnt(0);
	}

	Dead_Cnt++;
	Black_fade_rate += 0.01;
	HitStop::StartHitStop(10);
	
	//�c�@�\�����؂�ւ�鎞��SE�Đ�
	if (Dead_Cnt == 210)
	{
		//�U���̃T�E���h
		app_atomex_start(Player_Stock_Decrease_Sound);
	}



	if (300 < Dead_Cnt)
	{

		if (PlayerLife::GetLife() > 1)
		{
			Dead_Flag = true;
			PlayerStamina::SetPLayerDead(false);
		}
		else
		{
			Game_over_cnt++;
			Game_over_tama_cnt += 0.5;

			if (Game_over_text_cnt < 23)
			{
				Game_over_text_cnt += 0.4;
			}

			if (70 < Game_over_tama_cnt)
			{
				Game_over_tama_cnt = 36;
			}

			

		}
	}

	if (300 < Game_over_cnt)
	{
		Dead_Flag = true;
		PlayerStamina::SetPLayerDead(false);

	}

}

void dead_production::Draw()
{
	
	
	float fadeeasingRate = Ease::InCubic(Black_fade_rate);
	if (0.7 < fadeeasingRate)
	{
		fadeeasingRate = 0.7;
	}

	//�������̍����d�˂�
	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Black_texture);
	DrawSpriteOld(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 0.0, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), fadeeasingRate);

	// �X�P�[���������Ȃ��ƃI�u�W�F�N�g�̃T�C�Y�̕\��������������g��
	float scale = SCREEN_SCALE;

	// �X�N���[�������ʒu (�v���g�^�C�v�ł͏�Z���������ǁ@���񂩂���Z�ɂ��āj
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	Player& player = Player::GetInstance();//�V���O���g��

	b2Vec2 position;
	position.x = player.GetOutSidePlayerBody()->GetPosition().x;
	position.y = player.GetOutSidePlayerBody()->GetPosition().y;

	///--------------------------------------------------------------------------------------
	//�O�g
	// �v���C���[�ʒu���l�����ăX�N���[���␳��������
	//�擾����body�̃|�W�V�����ɑ΂���Box2d�X�P�[���̕␳��������
	float draw_x = ((position.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
	float draw_y = ((position.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

	if (120 < Dead_Cnt)
	{
	
		double fadeeasingRate = 0;
		Black_hole_rate += 0.03;
			
		fadeeasingRate = Ease::InCubic(Black_hole_rate);
		if (1 < fadeeasingRate)
		{
			fadeeasingRate = 1;

		}

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Black_hole_texture);

		DrawSprite(
			XMFLOAT2(draw_x, draw_y),
			0.0f,
			XMFLOAT2(2000 - (2000 * fadeeasingRate), 2000 - (2000 * fadeeasingRate))
		);
		DrawSprite(
			XMFLOAT2(draw_x, draw_y),
			0.0f,
			XMFLOAT2(4000, 4000)
		);

	}


	if (0.85 < Black_hole_rate)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Black_texture);
		DrawSprite(
			XMFLOAT2(draw_x, draw_y),
			0.0f,
			XMFLOAT2(2000, 2000)
		);
	}


	//�c�@�\��
	if (210 < Dead_Cnt)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_player_icon_texture);
		DrawSpriteOld(
			XMFLOAT2(SCREEN_WIDTH/2-150, SCREEN_HEIGHT/2),
			0.0f,
			XMFLOAT2(200, 200)
		);


		//�΂��Ă��\��
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_batten_texture);
		DrawSpriteOld(
			XMFLOAT2(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2),
			0.73f,
			XMFLOAT2(50, 50)
		);


		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_texture);


		int cnt = PlayerLife::GetLife();

		if ( Dead_Cnt<250)
		{
			
			DrawDividedSprite(XMFLOAT2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2), 0.0f, XMFLOAT2(100, 100), 10, 1, cnt, 1.0);
		}
		else
		{
			
			DrawDividedSprite(XMFLOAT2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2), 0.0f, XMFLOAT2(100, 100), 10, 1, cnt-1, 1.0);
		}

	}

	if (Game_over_cnt != 0)
	{

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_game_over_back_ground_texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
		);


		//�e�L�X�g�̕\��
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_game_over_text_texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2)-200),
			0.0f,
			XMFLOAT2(700, 300),5,5,Game_over_text_cnt,2.0f

		);


		if (Game_over_tama_cnt < 36)
		{
			//���܂����̕\��
			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_game_over_tama_start_texture);
			DrawDividedSprite(
				XMFLOAT2(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 100),
				0.0f,
				XMFLOAT2(400, 400), 6, 6, Game_over_tama_cnt, 2.0f
			);
		}
		else
		{
			//���܂����̕\��
			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_game_over_tama_loop_texture);
			DrawDividedSprite(
				XMFLOAT2(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 100),
				0.0f,
				XMFLOAT2(400, 400), 6, 6, Game_over_tama_cnt, 2.0f
			);


			
		}
	}

	
			
	

}

void dead_production::Finalize()
{
	if (g_Black_texture) UnInitTexture(g_Black_texture);
	if (g_Black_hole_texture) UnInitTexture(g_Black_hole_texture);

	if (g_player_icon_texture) UnInitTexture(g_player_icon_texture);

	if (g_batten_texture) UnInitTexture(g_batten_texture);

	if (g_number_texture) UnInitTexture(g_number_texture);

	if (g_game_over_back_ground_texture) UnInitTexture(g_game_over_back_ground_texture);
	if (g_game_over_tama_start_texture) UnInitTexture(g_game_over_tama_start_texture);
	if (g_game_over_tama_loop_texture) UnInitTexture(g_game_over_tama_loop_texture);
	if (g_game_over_text_texture) UnInitTexture(g_game_over_text_texture);
}

