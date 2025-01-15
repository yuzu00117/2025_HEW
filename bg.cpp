//-----------------------------------------------------------------------------------------------------
// #name bg.cpp
// #description �w�i����
// #make 2024/12/12�@�@�����F��
// #update 2024/12/12
// #comment  
//			�@�w�i�̑傫��1280*720���Ƒ������Ԃ����Ȃ��i�����j
//          
//----------------------------------------------------------------------------------------------------

#include "sprite.h"
#include "texture.h"
#include "renderer.h"
#include "Bg.h"
#include "player_position.h"


static ID3D11ShaderResourceView* g_Bg_Texture1 = NULL;//�w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Bg_Texture2 = NULL;//�w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Bg_Texture3 = NULL;//�w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Bg_Texture4 = NULL;//�w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Bg_Texture5 = NULL;//�w�i�̃e�N�X�`��
Bg* g_p_bg[3] = { nullptr };
b2Vec2 g_old_player_position;


// UV�X�N���[���pTu���W
static float g_UvScrollTu = 0.0f;

void Bg::Initialize()
{
	
	g_Bg_Texture1=	InitTexture(L"asset\\texture\\stage1_1\\background1.png");
	g_Bg_Texture2 = InitTexture(L"asset\\texture\\stage1_1\\background2.png");
	g_Bg_Texture3 = InitTexture(L"asset\\texture\\stage1_1\\background3.png");
	g_Bg_Texture4 = InitTexture(L"asset\\texture\\stage1_1\\background4.png");
	g_Bg_Texture5 = InitTexture(L"asset\\texture\\stage1_1\\background5.png");
	

	g_p_bg[0] = new Bg(XMFLOAT2(SCREEN_WIDTH / 2 * -1, SCREEN_HEIGHT / 2));
	g_p_bg[1] = new Bg(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	g_p_bg[2] = new Bg(XMFLOAT2(g_p_bg[1]->GetPosition().x * 3, SCREEN_HEIGHT / 2));

	g_old_player_position = b2Vec2(0.0, 0.0);
}

void Bg::Update()
{
	b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

	//��ʂ��E��
	if (player_position.x < g_old_player_position.x)
	{
		for (int i = 0; i < 3; i++)
		{
			float x = g_p_bg[i]->GetPosition().x + (g_old_player_position.x - player_position.x) * 100;
			g_p_bg[i]->SetPosition(XMFLOAT2(x, g_p_bg[i]->GetPosition().y));
		}
	}
	//��ʂ�����
	if (player_position.x > g_old_player_position.x)
	{
		for (int i = 0; i < 3; i++)
		{
			float x = g_p_bg[i]->GetPosition().x + (g_old_player_position.x - player_position.x) * 100;
			g_p_bg[i]->SetPosition(XMFLOAT2(x, g_p_bg[i]->GetPosition().y));
		}
	}
	//��ʂ����
	if (player_position.y > g_old_player_position.y)
	{
		for (int i = 0; i < 3; i++)
		{
			float y = g_p_bg[i]->GetPosition().y + (g_old_player_position.y - player_position.y) * 20;
			g_p_bg[i]->SetPosition(XMFLOAT2(g_p_bg[i]->GetPosition().x, y));
		}
	}
	//��ʂ�����
	if (player_position.y < g_old_player_position.y)
	{
		for (int i = 0; i < 3; i++)
		{
			float y = g_p_bg[i]->GetPosition().y + (g_old_player_position.y - player_position.y) * 20;
			g_p_bg[i]->SetPosition(XMFLOAT2(g_p_bg[i]->GetPosition().x, y));
		}
	}


	//�ʂ�߂�������W����O�ɂ��炷�i���[�v�����j
	for (int i = 0; i < 3; i++)
	{
		//�O���W��菬�����Ƃ��i�����ړ����Ă���Ƃ��j
		if (player_position.x < g_old_player_position.x)
		{
			if (g_p_bg[i]->GetPosition().x > SCREEN_WIDTH * 2)
			{
				float x = g_p_bg[i]->GetPosition().x - SCREEN_WIDTH * 3.0f;
				g_p_bg[i]->SetPosition(XMFLOAT2(x, g_p_bg[i]->GetPosition().y));
			}
		}
		//�O���W���傫���Ƃ��i�E���ړ����Ă���Ƃ��j
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
	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture1);

	DrawSprite(XMFLOAT2( g_p_bg[0]->GetPosition().x, g_p_bg[0]->GetPosition().y-440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH+300, SCREEN_HEIGHT+300));
	DrawSprite(XMFLOAT2(g_p_bg[1]->GetPosition().x, g_p_bg[1]->GetPosition().y-440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH+300, SCREEN_HEIGHT+300));
	DrawSprite(XMFLOAT2(g_p_bg[2]->GetPosition().x, g_p_bg[2]->GetPosition().y-440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH+300, SCREEN_HEIGHT+300));



	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture2);

	DrawSprite(XMFLOAT2(g_p_bg[0]->GetPosition().x, g_p_bg[0]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));
	DrawSprite(XMFLOAT2(g_p_bg[1]->GetPosition().x, g_p_bg[1]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));
	DrawSprite(XMFLOAT2(g_p_bg[2]->GetPosition().x, g_p_bg[2]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));



	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture3);

	DrawSprite(XMFLOAT2(g_p_bg[0]->GetPosition().x, g_p_bg[0]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));
	DrawSprite(XMFLOAT2(g_p_bg[1]->GetPosition().x, g_p_bg[1]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));
	DrawSprite(XMFLOAT2(g_p_bg[2]->GetPosition().x, g_p_bg[2]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));

	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture4);

	DrawSprite(XMFLOAT2(g_p_bg[0]->GetPosition().x, g_p_bg[0]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));
	DrawSprite(XMFLOAT2(g_p_bg[1]->GetPosition().x, g_p_bg[1]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));
	DrawSprite(XMFLOAT2(g_p_bg[2]->GetPosition().x, g_p_bg[2]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));


	// �V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture5);

	DrawSprite(XMFLOAT2(g_p_bg[0]->GetPosition().x, g_p_bg[0]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));
	DrawSprite(XMFLOAT2(g_p_bg[1]->GetPosition().x, g_p_bg[1]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));
	DrawSprite(XMFLOAT2(g_p_bg[2]->GetPosition().x, g_p_bg[2]->GetPosition().y - 440),
		0.0f,
		XMFLOAT2(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300));

}

void Bg::Finalize()
{
	UnInitTexture(g_Bg_Texture1);
	UnInitTexture(g_Bg_Texture2);
	UnInitTexture(g_Bg_Texture3);
	UnInitTexture(g_Bg_Texture4);
	UnInitTexture(g_Bg_Texture5);
}
