#include"Item_Coin_UI.h"
#include"texture.h"
#include"sprite.h"


static ID3D11ShaderResourceView* g_number_Texture = NULL;//�����̃e�N�X�`��

// �ÓI�����o�ϐ��̒�`�i�������j
int Item_Coin_UI::DrawCount = 0;

int Item_Coin_UI::max_coin_count = 30;

int Item_Coin_UI::now_get_coin__count = 0;

void Item_Coin_UI::Initialize()
{
	g_number_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_number.png");
}

void Item_Coin_UI::Draw()
{
	int Draw_count = Item_Coin_UI::GetDrawCount();


	if (Draw_count != 0)
	{

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);


		int cnt = now_get_coin__count;
		// �����̕\�� ���݂̗�
		for (int i = 0; i < 2; i++)
		{
			DrawDividedSprite(XMFLOAT2(100 - (i * 20), 700), 0.0f, XMFLOAT2(20, 20), 10, 1, cnt, 1.0);
			cnt /= 10;
		}

		int max_cnt = max_coin_count;
		for (int i = 0; i < 2; i++)
		{
			DrawDividedSprite(XMFLOAT2(150 - (i * 20), 700), 0.0f, XMFLOAT2(20, 20), 10, 1, max_cnt, 1.0);
			max_cnt /= 10;
		}

		DrawCount++;
		if (180 < DrawCount)
		{
			Item_Coin_UI::SetDrawCount(0);
		}
	}
}

void Item_Coin_UI::Finalize()
{

}