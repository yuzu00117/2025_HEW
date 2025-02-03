// ---------------------------------------------------------------------------------------------------- -
// #name gokai.cpp
// #description     gokaiのUI表示用
// #make 2025/02/02　永野義也
// #update 2024/02/02
// #comment 追加・修正予定
//
//----------------------------------------------------------------------------------------------------
#include"gokai.h"
#include"texture.h"
#include"sprite.h"


static ID3D11ShaderResourceView * g_number_Texture = NULL;//数字のテクスチャ

// 静的メンバ変数の定義（初期化）


int Gokai_UI::now_get_gokai_count = 0;  // 現在のコイン数

void Gokai_UI::Initialize()
{
	if (g_number_Texture == NULL)
	{
		g_number_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_number.png");
	}
}

void Gokai_UI::Draw()
{

		// シェーダリソースを設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);


		int cnt = now_get_gokai_count;
		// 左側の表示 現在の量
		for (int i = 0; i < 4; i++)
		{
			DrawDividedSprite(XMFLOAT2(1000 - (i * 20), 100), 0.0f, XMFLOAT2(20, 20), 10, 1, cnt, 1.0);
			cnt /= 10;
		}

	




}

void Gokai_UI::Finalize()
{
	UnInitTexture(g_number_Texture);
}