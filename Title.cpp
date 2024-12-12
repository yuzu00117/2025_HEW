

#include"title.h"
#include "sprite.h"
#include "texture.h"
#include"scene.h"
#include"Xinput_controller.h"
#include"keyboard.h"



//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_Texture = NULL;

//����������
void InitializeTitle(void)
{
	g_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_red.png");
}

//�I������
void FinalizeTitle(void)
{
	//�e�N�X�`���̉��
	UnInitTexture(g_Texture);
}

//�X�V����
void UpdateTitle(void) 
{
	//�Q�[���̃Z���N�g�Ɉڍs
	if (Keyboard_IsKeyDown(KK_SPACE))
	{
		ChangeScene(SCENE_GAMESELECT);
	}
	
}

//�`�揈��
void DrawTitle(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	DrawSprite({ SCREEN_XCENTER, SCREEN_YCENTER }, 0.0f, { SCREEN_WIDTH, SCREEN_HEIGHT });
}
