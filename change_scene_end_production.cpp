//-----------------------------------------------------------------------------------------------------
// #name change_scene_end_production.cpp
// #description    シーン移行の際の終わる側の演出
// #make 2025/02/15　永野義也
// #update 2025/02/15
// #comment 追加・修正予定
//
//-------------------------------------------------------------------------------------------------------
#include"change_scene_end_production.h"
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

//説明に使うテクスチャ
static ID3D11ShaderResourceView* g_Explanation_Texture = NULL;
static ID3D11ShaderResourceView* g_Explanation_BackGround_Texture = NULL;

float change_scene_end_production::Change_Scene_Cnt = 0;
float change_scene_end_production::Black_fade_rate = 0;
float change_scene_end_production::Black_hole_rate = 0;
float change_scene_end_production::Explanation_cnt = 0;
bool change_scene_end_production::fade_end_flag = false;
bool change_scene_end_production::Change_Flag = false;

void change_scene_end_production::Initialize()
{
	if (g_Black_texture == NULL)
	{
		g_Black_texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
		g_Black_hole_texture = InitTexture(L"asset\\texture\\sample_texture\\sample_fade_black.png");
		g_Explanation_Texture = InitTexture(L"asset\\texture\\Explanation_texture\\tips01.png");
		g_Explanation_BackGround_Texture = InitTexture(L"asset\\texture\\Explanation_texture\\ver01.png");
	}
}


void change_scene_end_production::Reset()
{
	Change_Scene_Cnt = 0;
	Black_fade_rate = 0;
	Black_hole_rate = 0;
	Explanation_cnt = 0;
	Change_Flag = false;
	fade_end_flag = false;
}

void change_scene_end_production::Update()
{
	Change_Scene_Cnt++;
	Black_fade_rate += 0.01;
	HitStop::StartHitStop(10);

}

void change_scene_end_production::Draw()
{
	if (Change_Scene_Cnt != 0)
	{

		float fadeeasingRate = Ease::InCubic(Black_fade_rate);
	
		if (3 < fadeeasingRate)
		{
			fadeeasingRate = 3;

			fade_end_flag = true;
		}
		float angle=45 * 180.0f / M_PI;

		//半透明の黒を重ねる
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Black_texture);
		DrawSpriteOld(XMFLOAT2(-1000+SCREEN_WIDTH/2  * fadeeasingRate, -1000+SCREEN_HEIGHT/2  * fadeeasingRate), angle, XMFLOAT2(SCREEN_WIDTH*2, SCREEN_HEIGHT*4), 1.0);

	}

	if (fade_end_flag == true)
	{
		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Explanation_BackGround_Texture);
		DrawSpriteOld(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 0.0, XMFLOAT2(SCREEN_WIDTH , SCREEN_HEIGHT), 1.0);

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Explanation_Texture);
		DrawSpriteOld(XMFLOAT2(SCREEN_WIDTH / 2 ,  SCREEN_HEIGHT / 2), 0.0, XMFLOAT2(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.6), 1.0);

		Explanation_cnt += 0.1;
	}

	if (15 < Explanation_cnt)
	{
		Change_Flag = true;
	}

}

void change_scene_end_production::Finalize()
{
	if (g_Black_texture) UnInitTexture(g_Black_texture);
	if (g_Black_hole_texture) UnInitTexture(g_Black_hole_texture);

	if (g_Explanation_Texture) UnInitTexture(g_Explanation_Texture);

	if (g_Explanation_BackGround_Texture) UnInitTexture(g_Explanation_BackGround_Texture);

	

}