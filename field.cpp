#include "field.h"
#include "sprite.h"
#include "texture.h"

//�}�N����`
#define RADIUS 300.0f
#define INNNER_RAD (((NUM_VERTEX - 2) * M_PI) / NUM_VERTEX)

//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture = NULL;

//����������
void InitField(void)
{
	g_Texture = InitTexture(L"asset\\texture\\BG.png");
}

//�I������
void UninitField(void)
{
	//�e�N�X�`���̉��
	UnInitTexture(g_Texture);
}

//�X�V����
void UpdateField(void)
{

}

//�`�揈��
void DrawField(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}
