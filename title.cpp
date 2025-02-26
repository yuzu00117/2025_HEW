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
static ID3D11ShaderResourceView* g_title_background_Texture = NULL;
static ID3D11ShaderResourceView* g_title_logo_text_Texture = NULL;
static ID3D11ShaderResourceView* g_title_logo_chain_Texture = NULL;
static ID3D11ShaderResourceView* g_title_logo_anchor_Texture = NULL;

//���肵�ăX�e�[�W�ɂƂԂ܂łɃe�N�X�`���Ȃ��Ȃ����ׂ����߂̂���
static ID3D11ShaderResourceView* g_black_Texture = NULL;





void TitleScene::Initialize()
{
	g_title_background_Texture = InitTexture(L"asset\\texture\\title_texture\\title_background.jpg");
	g_title_logo_text_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo_text.png");
	g_title_logo_chain_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo_chain.png");
	g_title_logo_anchor_Texture = InitTexture(L"asset\\texture\\title_texture\\title_logo_anchor.png");

	g_black_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
	

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

		scene_change_flag = true;
		
	};

	if (150 < scene_change_cnt)
	{
		SceneManager& sceneManager = SceneManager::GetInstance();
		sceneManager.ChangeScene(SCENE_STAGE_SELECT);
	}

	if (Keyboard_IsKeyDown(KK_R) || (state.buttonA))
	{
		text_sheet_cnt = 0;
	}




	//----------------------------------------------------------------------------------------
	//�����̃e�L�X�g�̃V�[�g�Ǘ�
	int max_text_sheet = 32;
	if (text_sheet_cnt <= max_text_sheet)
	{
		text_sheet_cnt += 0.5f;
	}


	//�t���O�@�`�F���W�V�V�[���̃t���O
	if (scene_change_flag == true)
	{
		//----------------------------------------------------------------------------------------
		//�`�F�C���̃V�[�g�J�E���g
		int max_chain_sheet = 47;
		if (chain_sheet_cnt <= max_chain_sheet)
		{
			chain_sheet_cnt += 0.5;
		}

		//----------------------------------------------------------------------------------------
		//�A���J�[�̃V�[�g�J�E���g
		int max_anchor_sheet = 47;
		if (anchor_sheet_cnt <= max_anchor_sheet)
		{
			anchor_sheet_cnt += 0.5;
		}

		scene_change_cnt++;

		
		if (black_fade_cnt<0.4)
		{
			black_fade_cnt += 0.005;
		}
		
	}



}

void TitleScene::Draw()
{

	//�o�b�t�@�N���A
	Clear();

	//2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);


	//�w�i
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

	if (g_black_Texture)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_black_Texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT),
			black_fade_cnt
		);
	}

	//���S�̃`�F�C��
	if (g_title_logo_chain_Texture != nullptr)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_logo_chain_Texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
			7, 7, chain_sheet_cnt
		);
	}

	//���S�̃e�L�X�g
	if (g_title_logo_text_Texture != nullptr)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_logo_text_Texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
			6,6, text_sheet_cnt
		);
	}

	//�A���J�[�̃e�L�X�g
	if (g_title_logo_anchor_Texture != nullptr)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_title_logo_anchor_Texture);

		DrawDividedSprite(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
			7, 7, anchor_sheet_cnt
		);
	}




	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();

}

void TitleScene::Finalize()
{
	if (g_title_background_Texture) UnInitTexture(g_title_background_Texture);
	if (g_title_logo_chain_Texture) UnInitTexture(g_title_logo_chain_Texture);
	if (g_title_logo_text_Texture) UnInitTexture(g_title_logo_text_Texture);
	if (g_title_logo_anchor_Texture) UnInitTexture(g_title_logo_anchor_Texture);
	if (g_black_Texture) UnInitTexture(g_black_Texture);



}