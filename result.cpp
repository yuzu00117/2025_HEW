#include "result.h"
#include "sprite.h"
#include "texture.h"
#include "keyboard.h"
#include "scene.h"




//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture = NULL;

//����������
void InitResult(void)
{
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");
}

//�I������
void UninitResult(void)
{
	//�e�N�X�`���̉��
	UnInitTexture(g_Texture);
}

//�X�V����
void UpdateResult(void)
{

	if (Keyboard_IsKeyDown(KK_C))
	{
		ChangeScene(SCENE_GAMESELECT);
	}

	if (Keyboard_IsKeyDown(KK_T))
	{
		ChangeScene(SCENE_TITLE);
	}

}

//�`�揈��
void DrawResult(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}
