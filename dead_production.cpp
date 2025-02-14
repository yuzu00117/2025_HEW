//-----------------------------------------------------------------------------------------------------
// #name dead_production.cpp
// #description    死亡演出
// #make 2025/02/08　永野義也
// #update 2025/02/08
// #comment 追加・修正予定
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

static ID3D11ShaderResourceView* g_Black_texture = NULL;//数字のテクスチャ
static ID3D11ShaderResourceView* g_Black_hole_texture = NULL;//数字のテクスチャ

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

	//半透明の黒を重ねる
	// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Black_texture);
	DrawSpriteOld(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 0.0, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), fadeeasingRate);

	// スケールをかけないとオブジェクトのサイズの表示が小さいから使う
	float scale = SCREEN_SCALE;

	// スクリーン中央位置 (プロトタイプでは乗算だったけど　今回から加算にして）
	b2Vec2 screen_center;
	screen_center.x = SCREEN_CENTER_X;
	screen_center.y = SCREEN_CENTER_Y;

	Player& player = Player::GetInstance();//シングルトン

	b2Vec2 position;
	position.x = player.GetOutSidePlayerBody()->GetPosition().x;
	position.y = player.GetOutSidePlayerBody()->GetPosition().y;

	///--------------------------------------------------------------------------------------
	//外枠
	// プレイヤー位置を考慮してスクロール補正を加える
	//取得したbodyのポジションに対してBox2dスケールの補正を加える
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

		// シェーダリソースを設定
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
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Black_texture);
		DrawSprite(
			XMFLOAT2(draw_x, draw_y),
			0.0f,
			XMFLOAT2(2000, 2000)
		);
	}

	if (210 < Dead_Cnt)
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
	}
}