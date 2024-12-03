#include "gameover.h"
#include "sprite.h"
#include "texture.h"
#include "keyboard.h"
#include "scene.h"




//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture = NULL;

//����������
void InitGameOver(void)
{
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
}

//�I������
void UninitGameOver(void)
{
	//�e�N�X�`���̉��
	UnInitTexture(g_Texture);
}

//�X�V����
void UpdateGameOver(void)
{
	if (Keyboard_IsKeyDown(KK_T))
	{
		ChangeScene(SCENE_TITLE);
	}

	if (Keyboard_IsKeyDown(KK_R))
	{
		ChangeScene(SCENE_GAMESELECT);
	}
}

//�`�揈��
void DrawGameOver(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}
