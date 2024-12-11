#include "gameover.h"
#include "sprite.h"
#include "texture.h"
#include "keyboard.h"
#include "scene.h"




//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture = NULL;

//����������
void InitializeGameOver(void)
{
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_Yellow.png");
}

//�I������
void FinalizeGameOver(void)
{
	//�e�N�X�`���̉��
	UnInitTexture(g_Texture);
}

//�X�V����
void UpdateGameOver(void)
{
	if (Keyboard_IsKeyDown(KK_M))
	{
		ChangeScene(SCENE_TITLE);
	}

	if (Keyboard_IsKeyDown(KK_N))
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
