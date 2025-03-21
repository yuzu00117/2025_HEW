//-----------------------------------------------------------------------------------------------------
// #name player_life.cpp
// #description プレイヤーの残機表示
// #make 2024/12/15　　今村友哉
// #update 2024/12/15
// #comment 
//          
//----------------------------------------------------------------------------------------------------

#include "sprite.h"
#include "texture.h"
#include "renderer.h"
#include "keyboard.h"
#include "player_life.h"
#include "player_stamina.h"

#define PLAYER_LIFE (3) //プレイヤーの残機の初期値

static ID3D11ShaderResourceView* g_life_Texture = NULL;//残機のテクスチャ

float PlayerLife::m_life = PLAYER_LIFE;//プレイヤーの残機



void PlayerLife::Initialize()
{
	g_life_Texture = InitTexture(L"asset\\texture\\life.png");
	m_life = PLAYER_LIFE;

}

void PlayerLife::Finalize()
{
	if (g_life_Texture) UnInitTexture(g_life_Texture);

}

void PlayerLife::Update()
{
#ifdef _DEBUG
	//テスト用　残機を減らす
	if (Keyboard_IsKeyDown(KK_L))
	{
		m_life -= 1;
	}
#endif // DEBUG
}

void PlayerLife::Draw()
{
	//GetDeviceContext()->PSSetShaderResources(0, 1, &g_life_Texture);

	////																									　		全体の個数
	//DrawDividedSprite(XMFLOAT2(SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT - 50.0f), 0.0f, XMFLOAT2(50.0f, 50.0f), 5, 2, m_life, 1.0f);
}