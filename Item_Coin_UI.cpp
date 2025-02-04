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

// 静的メンバ変数の定義（初期化）
int Item_Coin_UI::DrawCount = 0;			// 描画カウント
int Item_Coin_UI::max_coin_count = 30;		// 最大コイン数
int Item_Coin_UI::now_get_coin_count = 0;  // 現在のコイン数

void Item_Coin_UI::Initialize()
{
	if (g_number_Texture == NULL)
	{
		g_number_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_number.png");
	}
}

void Item_Coin_UI::Draw()
{
	// 描画カウントが0の場合は描画しない
	int Draw_count = Item_Coin_UI::GetDrawCount();


	if (Draw_count != 0)
	{

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);


		int cnt = now_get_coin_count;
		// 左側の表示 現在の量
		for (int i = 0; i < 2; i++)
		{
			DrawDividedSprite(XMFLOAT2(100 - (i * 20), 100), 0.0f, XMFLOAT2(20, 20), 10, 1, cnt, 1.0);
			cnt /= 10;
		}

		int max_cnt = max_coin_count;
		for (int i = 0; i < 2; i++)
		{
			DrawDividedSprite(XMFLOAT2(150 - (i * 20), 100), 0.0f, XMFLOAT2(20, 20), 10, 1, max_cnt, 1.0);
			max_cnt /= 10;
		}

		// 描画カウントを減らす
		Item_Coin_UI::SetDrawCount(GetDrawCount()-1);
		
	}
}

void Item_Coin_UI::Finalize()
{
	UnInitTexture(g_number_Texture);
}