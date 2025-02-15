//-----------------------------------------------------------------------------------------------------
// #name Item_Coin_UI.cpp
// #description     coinのUI表示用
// #make 2024/12/28　永野義也
// #update 2024/02/01
// #comment 追加・修正予定
//
//----------------------------------------------------------------------------------------------------
#include"Item_Coin_UI.h"
#include"texture.h"
#include"sprite.h"


static ID3D11ShaderResourceView* g_number_Texture = NULL;//数字のテクスチャ

static ID3D11ShaderResourceView* g_coin_Texture = NULL;//コインのテクスチャ

static ID3D11ShaderResourceView* g_coin_effect_Texture = NULL;//コインのテクスチャ



// 静的メンバ変数の定義（初期化）
int Item_Coin_UI::DrawCount = 0;			// 描画カウント
int Item_Coin_UI::max_coin_count = 30;		// 最大コイン数
int Item_Coin_UI::now_get_coin_count = 0;  // 現在のコイン数
float Item_Coin_UI::coin_effect_frame = 0;

void Item_Coin_UI::Initialize()
{
	if (g_number_Texture == NULL)
	{
		g_number_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_number.png");
		g_coin_Texture	 = InitTexture(L"asset\\texture\\sample_texture\\sample_coin.png");
		g_coin_effect_Texture = InitTexture(L"asset\\texture\\stage_select_texture\\stage_select_coin_effect3.png");
	}
}

void Item_Coin_UI::Draw()
{
	// シェーダリソースを設定
	


	//----------------------------------------------------------------------------
	//コインを描画　テクスチャね
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_coin_Texture);
	DrawSpriteOld(XMFLOAT2(1150, 100), 0.0f, XMFLOAT2(35, 35), 1.0f);

	//-------------------------------------------------------------------------
	int coin_height = 0;

	if (DrawCount != 0)
	{

		if (25 < DrawCount)
		{
			coin_height = 20;
		}

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_coin_effect_Texture);
		DrawDividedSprite(XMFLOAT2(1160, 110), 0.0f, XMFLOAT2(50, 50),5,5,coin_effect_frame,1.0f);

		coin_effect_frame += 0.3;
		DrawCount--;
	}
	else
	{
		coin_effect_frame = 0;
	}


	//数字の表示時間

	// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);


	int cnt = now_get_coin_count;
	// 左側の表示 現在の量
	for (int i = 0; i < 2; i++)
	{
		DrawDividedSprite(XMFLOAT2(1200 - (i * 20), 100-coin_height), 0.0f, XMFLOAT2(20, 20), 10, 1, cnt, 1.0);
		cnt /= 10;
	}

	int max_cnt = max_coin_count;
	for (int i = 0; i < 2; i++)
	{
		DrawDividedSprite(XMFLOAT2(1250 - (i * 20), 100), 0.0f, XMFLOAT2(20, 20), 10, 1, max_cnt, 1.0);
		max_cnt /= 10;
	}
	


	
		
	
}

void Item_Coin_UI::Finalize()
{
	if (g_number_Texture != NULL)
	{
		UnInitTexture(g_number_Texture);
		g_number_Texture = NULL;
		UnInitTexture(g_coin_effect_Texture);
		g_coin_effect_Texture = NULL;
		UnInitTexture(g_coin_Texture);
		g_coin_Texture = NULL;
	}
}