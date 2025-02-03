#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"
#include"Item_Coin_UI.h"
#include"1-1_boss.h"


//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_result_Texture = NULL;

//�X�R�A�����N�̃e�N�X�`��
ID3D11ShaderResourceView* coin_score_texture = NULL;		//�R�C���X�R�A�����N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* clear_time_score_texture = NULL;	//�R�C���X�R�A�����N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* g_score_rank_C_Texture = NULL;	//C�����N
ID3D11ShaderResourceView* g_score_rank_B_Texture = NULL;	//B�����N
ID3D11ShaderResourceView* g_score_rank_A_Texture = NULL;	//A�����N
ID3D11ShaderResourceView* g_score_rank_S_Texture = NULL;	//S�����N

//�X�R�A�����N�̕`��T�C�Y
#define SCORE_RANK_SIZE (100)

void ResulttScene::Initialize()
{
	//���U���g��ʂ̔w�i�e�N�X�`���̓ǂݍ���
	g_result_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_result.png");
	

	//�X�R�A�����N�̃e�N�X�`���̓ǂݍ���
	g_score_rank_C_Texture = InitTexture(L"asset\\texture\\sample_texture\\stamp01_c.png");
	g_score_rank_B_Texture = InitTexture(L"asset\\texture\\sample_texture\\stamp01_b.png");
	g_score_rank_A_Texture = InitTexture(L"asset\\texture\\sample_texture\\stamp01_a.png");
	g_score_rank_S_Texture = InitTexture(L"asset\\texture\\sample_texture\\stamp01_s.png");
}

void ResulttScene::Update()
{
    // �R���g���[���[�̓��͂̎󂯎��
    ControllerState state = GetControllerInput();

    // �R�C���̖������擾
    int coinCount = Item_Coin_UI::GetNowCoinCount();
    int scorePoints = 0;

    // �R�C���̖����ɉ����ăX�R�A�����N�̃e�N�X�`���ƃ|�C���g��ݒ�
    if (coinCount >= 30) {
        coin_score_texture = g_score_rank_A_Texture;
        scorePoints = 100;
    } else if (coinCount >= 20) {
        coin_score_texture = g_score_rank_B_Texture;
        scorePoints = 75;
    } else if (coinCount >= 10) {
        coin_score_texture = g_score_rank_C_Texture;
        scorePoints = 50;
    } else {
        coin_score_texture = g_score_rank_C_Texture; // �f�t�H���g��C�����N
        scorePoints = 50;
    }

    // �X�R�A�|�C���g��ϐ��ɉ��Z
    total_score_points += scorePoints;



    // �{�X�̃N���A�^�C�����擾
	Boss_1_1& boss = Boss_1_1::GetInstance();
    float bossClearTime = boss.GetBossElapsedTime();

    // �N���A�^�C���ɉ����������N�ƃX�R�A�|�C���g�̐ݒ�
    if (bossClearTime >= 180.0f) { // 3���ȏ�
        clear_time_score_texture = g_score_rank_C_Texture;
        scorePoints = 50;
    } else if (bossClearTime >= 120.0f) { // 2���ȏ�
        clear_time_score_texture = g_score_rank_B_Texture;
        scorePoints = 75;
    } else { // 2������
        clear_time_score_texture = g_score_rank_A_Texture;
        scorePoints = 100;
    }

    // �X�R�A�|�C���g��ϐ��ɉ��Z
    total_score_points += scorePoints;

    if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
    {
        SceneManager& sceneManager = SceneManager::GetInstance();
        sceneManager.ChangeScene(SCENE_STAGE_SELECT);
    }
}

void ResulttScene::Draw()
{

	//�o�b�t�@�N���A
	Clear();

	//2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);

	//���U���g��ʂ̔w�i�̕`��
	if (g_result_Texture != nullptr)
	{
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_result_Texture);

		DrawSpriteOld(
			XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
			0.0f,
			XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
		);
	}

	//�{�X�̃N���A�^�C���X�R�A��`��
	//�V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &clear_time_score_texture);

    if (clear_time_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(550, 325), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
			0.0f,
			XMFLOAT2(SCORE_RANK_SIZE, SCORE_RANK_SIZE)
		);
    }

	//�X�R�A�����N��`��
	//�V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &coin_score_texture);

    if (coin_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(550, 245), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
			0.0f,
			XMFLOAT2(SCORE_RANK_SIZE, SCORE_RANK_SIZE)
		);
    }


	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();

}

void ResulttScene::Finalize()
{
	UnInitTexture(g_result_Texture);
}