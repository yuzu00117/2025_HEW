//-----------------------------------------------------------------------------------------------------
// #name player_life.cpp
// #description �v���C���[�̎c�@�\��
// #make 2024/12/15�@�@�����F��
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


static ID3D11ShaderResourceView* g_life_Texture = NULL;//�c�@�̃e�N�X�`��
float PlayerLife::m_life  = 9;//�v���C���[�̎c�@



void PlayerLife::Initialize()
{
	g_life_Texture = InitTexture(L"asset\\texture\\life.png");

}

void PlayerLife::Finalize()
{
	UnInitTexture(g_life_Texture);
}

void PlayerLife::Update()
{
	//�e�X�g�p�@�c�@�����炷
	if (Keyboard_IsKeyDown(KK_L))
	{
		m_life -= 1;
	}
}

void PlayerLife::Draw()
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_life_Texture);

	//																									�@		�S�̂̌�
	DrawDividedSprite(XMFLOAT2(SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT - 50.0f), 0.0f, XMFLOAT2(50.0f, 50.0f), 5, 2, m_life, 1.0f);
}