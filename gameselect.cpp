#include "gameselect.h"
#include "sprite.h"
#include "texture.h"
#include "keyboard.h"
#include "scene.h"




//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture = NULL;

//����������
void InitGameSelect(void)
{
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");
}

//�I������
void UninitGameSelect(void)
{
	//�e�N�X�`���̉��
	UnInitTexture(g_Texture);
}

//�X�V����
void UpdateGameSelect(void)
{
	if (Keyboard_IsKeyDown(KK_ENTER))
	{
		ChangeScene(SCENE_GAME);
	}
}

//�`�揈��
void DrawGameSelect(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}