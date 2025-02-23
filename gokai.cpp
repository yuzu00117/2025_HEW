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


int Gokai_UI::now_get_gokai_count = 0;  // 現在の豪快値	
int Gokai_UI::gokai_when_respawn = 0;  // リスポン時の豪快値	

void Gokai_UI::Initialize()
{
	//メンバー変数の初期化
	now_get_gokai_count = 0;  // 現在の豪快値	
	gokai_when_respawn = 0;  // リスポン時の豪快値	

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
		for (int i = 0; i < 5; i++)
		{
			DrawDividedSprite(XMFLOAT2(1250 - (i * 40), 40), 0.0f, XMFLOAT2(40, 40), 10, 1, cnt, 1.0);
			cnt /= 10;
		}

	




}

void Gokai_UI::Finalize()
{
	if (g_number_Texture != NULL)
	{
		UnInitTexture(g_number_Texture);
		g_number_Texture = NULL;
	}
}