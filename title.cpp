//-----------------------------------------------------------------------------------------------------
// #name title.cpp
// #description �^�C�g����������[
// #make 2024/12/17�@�@�i��`��
// #update 2024/12/17
// #comment �ǉ��E�C���\��
//          �E�^�C�g����������ˁ[
//           
//----------------------------------------------------------------------------------------------------

#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"


//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_title_background_Texture = NULL;
ID3D11ShaderResourceView* g_title_Texture = NULL;


float sheet_cnt = 0;

void TitleScene::Initialize()
{
	g_title_background_Texture = InitTexture(L"asset\\texture\\title_texture\\title_background.jpg");
	g_title_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo.png");

	

	//�S�Ẳ����~�߂�
	app_atomex_stop_player();
	//�^�C�g����BGM��������
	app_atomex_start(TITLE_BGM);
}


void TitleScene::Update()
{
	//�R���g���[���[�̓��͂̎󂯎��
	ControllerState state = GetControllerInput();

	if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
	{
		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_STAGE_SELECT);
	};

	if (Keyboard_IsKeyDown(KK_R) || (state.buttonA))
	{
		sheet_cnt = 0;
	}

	int max_sheet = 62;

	if (sheet_cnt <= max_sheet)
	{
		sheet_cnt += 0.5f;
	}
}

void TitleScene::Draw()
{

	//�o�b�t�@�N���A
	Clear();

	//2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);

	if (g_title_background_Texture != nullptr)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_background_Texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
		);
	}

	if (g_title_Texture != nullptr)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_Texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
			8,8, sheet_cnt
		);
	}

	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();

}

void TitleScene::Finalize()
{
	if (g_title_background_Texture) UnInitTexture(g_title_background_Texture);
	if (g_title_Texture) UnInitTexture(g_title_Texture);

}