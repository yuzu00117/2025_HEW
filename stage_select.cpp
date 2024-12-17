#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"


//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_stage_select_Texture = NULL;

void StageSelectScene::Initialize()
{
	g_stage_select_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_stage_select.png");
}

void StageSelectScene::Update()
{
	//�R���g���[���[�̓��͂̎󂯎��
	ControllerState state = GetControllerInput();

	if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
	{
		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_GAME);
	};
}

void StageSelectScene::Draw()
{

	//�o�b�t�@�N���A
	Clear();

	//2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);

	if (g_stage_select_Texture != nullptr)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_stage_select_Texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
		);
	}

	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();

}

void StageSelectScene::Finalize()
{
	UnInitTexture(g_stage_select_Texture);
}