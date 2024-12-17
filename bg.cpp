//-----------------------------------------------------------------------------------------------------
// #name bg.cpp
// #description 背景処理
// #make 2024/12/12　　今村友哉
// #update 2024/12/12
// #comment  
//			　背景の大きさ1280*720だと多分隙間あかない（多分）
//          
//----------------------------------------------------------------------------------------------------

#include "sprite.h"
#include "texture.h"
#include "renderer.h"
#include "Bg.h"
#include "player_position.h"


static ID3D11ShaderResourceView* g_Bg_Texture = NULL;//背景のテクスチャ
Bg* g_p_bg[3] = { nullptr };
b2Vec2 g_old_player_position;


// UVスクロール用Tu座標
static float g_UvScrollTu = 0.0f;

void Bg::Initialize()
{
	g_Bg_Texture = InitTexture(L"asset\\texture\\haikei.png");

	g_p_bg[0] = new Bg(XMFLOAT2(SCREEN_WIDTH / 2 * -1, SCREEN_HEIGHT / 2));
	g_p_bg[1] = new Bg(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	g_p_bg[2] = new Bg(XMFLOAT2(g_p_bg[1]->GetPosition().x * 3, SCREEN_HEIGHT / 2));

	g_old_player_position = b2Vec2(0.0, 0.0);
}

void Bg::Update()
{
	b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

	//画面を右に
	if (player_position.x < g_old_player_position.x)
	{
		for (int i = 0; i < 3; i++)
		{
			float x = g_p_bg[i]->GetPosition().x + (g_old_player_position.x - player_position.x) * 300;
			g_p_bg[i]->SetPosition(XMFLOAT2(x, g_p_bg[i]->GetPosition().y));
		}
	}
	//画面を左に
	if (player_position.x > g_old_player_position.x)
	{
		for (int i = 0; i < 3; i++)
		{
			float x = g_p_bg[i]->GetPosition().x + (g_old_player_position.x - player_position.x) * 300;
			g_p_bg[i]->SetPosition(XMFLOAT2(x, g_p_bg[i]->GetPosition().y));
		}
	}
	//画面を上に
	if (player_position.y > g_old_player_position.y)
	{
		for (int i = 0; i < 3; i++)
		{
			float y = g_p_bg[i]->GetPosition().y + (g_old_player_position.y - player_position.y) * 20;
			g_p_bg[i]->SetPosition(XMFLOAT2(g_p_bg[i]->GetPosition().x, y));
		}
	}
	//画面を下に
	if (player_position.y < g_old_player_position.y)
	{
		for (int i = 0; i < 3; i++)
		{
			float y = g_p_bg[i]->GetPosition().y + (g_old_player_position.y - player_position.y) * 20;
			g_p_bg[i]->SetPosition(XMFLOAT2(g_p_bg[i]->GetPosition().x, y));
		}
	}


	//通り過ぎたら座標を手前にずらす（ループ処理）
	for (int i = 0; i < 3; i++)
	{
		//前座標より小さいとき（左側移動しているとき）
		if (player_position.x < g_old_player_position.x)
		{
			if (g_p_bg[i]->GetPosition().x > SCREEN_WIDTH * 2)
			{
				float x = g_p_bg[i]->GetPosition().x - SCREEN_WIDTH * 3.0f;
				g_p_bg[i]->SetPosition(XMFLOAT2(x, g_p_bg[i]->GetPosition().y));
			}
		}
		//前座標より大きいとき（右側移動しているとき）
		if (player_position.x > g_old_player_position.x)
		{
			if (g_p_bg[i]->GetPosition().x < -SCREEN_WIDTH)
			{
				float x = g_p_bg[i]->GetPosition().x + SCREEN_WIDTH * 3.0f;
				g_p_bg[i]->SetPosition(XMFLOAT2(x, g_p_bg[i]->GetPosition().y));
			}


		}


	}

	g_old_player_position = player_position;
}

void Bg::Draw()
{
	// シェーダリソースを設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture);

	DrawSprite(g_p_bg[0]->GetPosition(),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));
	DrawSprite(g_p_bg[1]->GetPosition(),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));
	DrawSprite(g_p_bg[2]->GetPosition(),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

}

void Bg::Finalize()
{
	UnInitTexture(g_Bg_Texture);
}
