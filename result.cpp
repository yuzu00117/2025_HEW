#include "result.h"
#include "sprite.h"
#include "texture.h"
#include "keyboard.h"
#include "scene.h"




//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture = NULL;

//����������
void InitializeResult(void)
{
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_green.png");
}

//�I������
void FinalizeResult(void)
{
	//�e�N�X�`���̉��
	UnInitTexture(g_Texture);
}

//�X�V����
void UpdateResult(void)
{
	//�Q�[���̃Z���N�g�Ɉڍs
	if (Keyboard_IsKeyDown(KK_N))
	{
		ChangeScene(SCENE_GAMESELECT);
	}

	//�^�C�g���Ɉڍs
	if (Keyboard_IsKeyDown(KK_M))
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
