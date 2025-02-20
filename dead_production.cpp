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

static ID3D11ShaderResourceView* g_Black_texture = NULL;//���̃e�N�X�`��
static ID3D11ShaderResourceView* g_Black_hole_texture = NULL;//�^�񒆂��󂢂Ă鍕�̃e�N�X�`��

static ID3D11ShaderResourceView* g_player_icon_texture = NULL;//�v���C���[�̃A�C�R��

static ID3D11ShaderResourceView* g_batten_texture = NULL;//�^�񒆂̂΂��Ă�

static ID3D11ShaderResourceView* g_number_texture = NULL;//�����̃e�N�X�`



float dead_production::Dead_Cnt = 0;
float dead_production::Black_fade_rate = 0;
float dead_production::Black_hole_rate = 0;
bool dead_production::Dead_Flag = false;

void dead_production::Initialize()
{
	if (g_Black_texture == NULL)
	{
		g_Black_texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
		g_Black_hole_texture = InitTexture(L"asset\\texture\\sample_texture\\sample_fade_black.png");
		g_player_icon_texture= InitTexture(L"asset\\texture\\UI_soul_gage\\player_icon.png");
		g_batten_texture = InitTexture(L"asset\\texture\\sample_texture\\batten.png");

		g_number_texture= InitTexture(L"asset\\texture\\sample_texture\\sample_number_white.png");
	}
}


void dead_production::Reset()
{
	Dead_Cnt = 0;
	Black_fade_rate = 0;
	Black_hole_rate = 0;
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

	if (300 < Dead_Cnt)
	{
		Dead_Flag = true;
	
	

		PlayerStamina::SetPLayerDead(false);
	}
			
	

}

void dead_production::Finalize()
{
	if (g_Black_texture != NULL)
	{
		UnInitTexture(g_Black_texture);
		g_Black_texture = NULL;

		UnInitTexture(g_Black_hole_texture);
		g_Black_hole_texture = NULL;

		UnInitTexture(g_player_icon_texture);
		g_player_icon_texture = NULL;

		UnInitTexture(g_number_texture);
		g_number_texture = NULL;
		
	}
}