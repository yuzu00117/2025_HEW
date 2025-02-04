#include"scene.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"
#include"Item_Coin_UI.h"
#include"1-1_boss.h"
#include"gokai.h"


//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_result_Texture = NULL;

//�X�R�A�����N�̃e�N�X�`��
ID3D11ShaderResourceView* total_score_texture = NULL;		//�g�[�^�������N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* coin_score_texture = NULL;		//�R�C�������N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* clear_time_score_texture = NULL;	//�R�C�������N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* gokai_score_texture = NULL;       //���������N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* g_score_rank_C_Texture = NULL;	//C�����N
ID3D11ShaderResourceView* g_score_rank_B_Texture = NULL;	//B�����N
ID3D11ShaderResourceView* g_score_rank_A_Texture = NULL;	//A�����N
ID3D11ShaderResourceView* g_score_rank_S_Texture = NULL;	//S�����N

//�X�R�A�����N�̕`��T�C�Y
#define RANK_SIZE (110)
#define TOTAL_RANK_SIZE (200)

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
    //�R���g���[���[�̓��͂̎󂯎��
    ControllerState state = GetControllerInput();

    //�X�R�A�|�C���g�̈ꎞ�i�[�p�ϐ�
    int total_score_points = 0;



    //�����x���擾���āA�����N�ɔ��f����
    int gokai_count = Gokai_UI::GetNowGokaiCount(); //�����x���擾
    int gokai_score_points = 0; //�����x�̃X�R�A�|�C���g�i�[�p

    //�����x�ɉ����č��������N�̃e�N�X�`���ƃ|�C���g��ݒ�
    if (gokai_count >= 3000) {
        gokai_score_texture = g_score_rank_A_Texture;
        gokai_score_points = 100;
    }
    else if (gokai_count >= 2000)
    {
        gokai_score_texture = g_score_rank_B_Texture;
        gokai_score_points = 75;
    } else {
        gokai_score_texture = g_score_rank_C_Texture;
        gokai_score_points = 50;
    }



    //�R�C���̖������擾���āA�����N�ɔ��f
    int coin_count = Item_Coin_UI::GetNowCoinCount(); //�R�C���̖������擾
    int coin_score_points = 0; //�R�C���̃X�R�A�|�C���g�i�[�p

    //�R�C���̖����ɉ����ăX�R�A�����N�̃e�N�X�`���ƃ|�C���g��ݒ�
    if (coin_count >= 30)
    {
        coin_score_texture = g_score_rank_A_Texture;
        coin_score_points = 100;
    }
    else if (coin_count >= 20)
    {
        coin_score_texture = g_score_rank_B_Texture;
        coin_score_points = 75;
    } else {
        coin_score_texture = g_score_rank_C_Texture;
        coin_score_points = 50;
    }



    //�{�X�̃N���A�^�C�����擾���āA�����N�ɔ��f
	Boss_1_1& boss = Boss_1_1::GetInstance(); //�{�X�̃C���X�^���X���擾
    float boss_clear_time = boss.GetBossElapsedTime(); //�{�X�̃N���A�^�C�����擾
    int boss_score_points = 0; //�{�X�̃X�R�A�|�C���g�i�[�p

    //�N���A�^�C���ɉ����������N�ƃX�R�A�|�C���g�̐ݒ�
    if (boss_clear_time >= 180.0f)
    { //3���ȏ�
        clear_time_score_texture = g_score_rank_C_Texture;
        boss_score_points = 50;
    }
    else if (boss_clear_time >= 120.0f)
    { //2���ȏ�
        clear_time_score_texture = g_score_rank_B_Texture;
        boss_score_points = 75;
    } else { //2������
        clear_time_score_texture = g_score_rank_A_Texture;
        boss_score_points = 100;
    }

    //�g�[�^���̃X�R�A�|�C���g���v�Z
    total_score_points = (gokai_score_points + coin_score_points + boss_score_points);

    //�g�[�^���̃X�R�A�|�C���g�ɉ����ă����N��ݒ�
    if (total_score_points >= 300)
    { //300�ȏ�Ȃ�S�����N
        total_score_texture = g_score_rank_S_Texture;
    }
    else if (total_score_points >= 250)
    { //250�ȏ�Ȃ�A�����N
        total_score_texture = g_score_rank_A_Texture;
    }
    else if (total_score_points >= 200)
    { //200�ȏ�Ȃ�B�����N
        total_score_texture = g_score_rank_B_Texture;
    } else { //200�ȉ��Ȃ�C�����N
        total_score_texture = g_score_rank_C_Texture;
    }

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

    //�����x�̃����N��`��
	//�V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &gokai_score_texture);

    if (gokai_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(600, 160), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
			0.0f,
			XMFLOAT2(RANK_SIZE, RANK_SIZE)
		);
    }

	//�R�C���̃����N��`��
	//�V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &coin_score_texture);

    if (coin_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(600, 240), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
			0.0f,
			XMFLOAT2(RANK_SIZE, RANK_SIZE)
		);
    }

    //�{�X�̃N���A�^�C�������N��`��
	//�V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &clear_time_score_texture);

    if (clear_time_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(600, 320), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
			0.0f,
			XMFLOAT2(RANK_SIZE, RANK_SIZE)
		);
    }

    //�g�[�^���̃����N��`��
	//�V�F�[�_���\�[�X��ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &total_score_texture);

    if (total_score_texture != NULL) {
        DrawSpriteOld(
			XMFLOAT2(185, 620), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
			0.0f,
			XMFLOAT2(TOTAL_RANK_SIZE, TOTAL_RANK_SIZE)
		);
    }

	//�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();

}

void ResulttScene::Finalize()
{
	UnInitTexture(g_result_Texture);
}