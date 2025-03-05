// ---------------------------------------------------------------------------------------------------- -
// #name result.cpp
// #description     ���U���g���
// #make 2025/02/02�@�i��`��
// #update 2024/02/27 �����A
// #comment �ǉ��E�C���\��
//----------------------------------------------------------------------------------------------------
#include"result.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"
#include"Item_Coin_UI.h"
#include"1-1_boss.h"
#include"gokai.h"


//�Q�[���V�[���̉�ʂ��i�[����e�N�X�`��
ID3D11ShaderResourceView* g_GameScreen_Texture = NULL;

//�e�N�X�`���̃_�E�����[�h �O���[�o���ϐ��ɂ��Ă�
ID3D11ShaderResourceView* g_result_Texture = NULL;
ID3D11ShaderResourceView* g_number_Texture = NULL;
ID3D11ShaderResourceView* g_colon_Texture = NULL;
ID3D11ShaderResourceView* g_gage_Texture = NULL;
ID3D11ShaderResourceView* g_gage_frame_Texture = NULL;
ID3D11ShaderResourceView* g_gage_frame2_Texture = NULL;
ID3D11ShaderResourceView* g_gage_points_Texture = NULL;

//�X�R�A�����N�̃e�N�X�`��
ID3D11ShaderResourceView* total_score_texture = NULL;		//�g�[�^�������N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* coin_score_texture = NULL;		//�R�C�������N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* clear_time_score_texture = NULL;	//�R�C�������N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* gokai_score_texture = NULL;       //���������N�̃e�N�X�`���i�[�p
ID3D11ShaderResourceView* g_score_rank_C_stamp_Texture = NULL;	//C�����N�i�X�^���v�j
ID3D11ShaderResourceView* g_score_rank_B_stamp_Texture = NULL;	//B�����N�i�X�^���v�j
ID3D11ShaderResourceView* g_score_rank_A_stamp_Texture = NULL;	//A�����N�i�X�^���v�j
ID3D11ShaderResourceView* g_score_rank_S_stamp_Texture = NULL;	//S�����N�i�X�^���v�j
ID3D11ShaderResourceView* g_score_rank_C_Texture = NULL;	//C�����N�i�X�^���v�j
ID3D11ShaderResourceView* g_score_rank_B_Texture = NULL;	//C�����N�i�X�^���v�j
ID3D11ShaderResourceView* g_score_rank_A_Texture = NULL;	//C�����N�i�X�^���v�j
ID3D11ShaderResourceView* g_score_rank_S_Texture = NULL;	//C�����N�i�X�^���v�j

int g_wait_cnt = 0;
int g_wait = 45;


void ResultScene::Initialize()
{
    //���U���g��ʂ̔w�i�e�N�X�`���̓ǂݍ���
    g_GameScreen_Texture=InitTexture(L"asset\\texture\\result_texture\\remains_background02.png");
    g_result_Texture = InitTexture(L"asset\\texture\\result_texture\\result_back_test2.png");
    //�����p�e�N�X�`���̓ǂݍ���
    g_number_Texture = InitTexture(L"asset\\texture\\result_texture\\sample_number.png");
    g_colon_Texture = InitTexture(L"asset\\texture\\result_texture\\sample_colon.png");

    //�Q�[�W�p�e�N�X�`��
    g_gage_Texture = InitTexture(L"asset\\texture\\result_texture\\score_gauge.png");
    g_gage_frame_Texture = InitTexture(L"asset\\texture\\result_texture\\score_gauge_frame.png");
    g_gage_frame2_Texture = InitTexture(L"asset\\texture\\result_texture\\score_gauge_frame2.png");
    g_gage_points_Texture = InitTexture(L"asset\\texture\\result_texture\\score_gauge_points.png");

    //�����N�̃e�N�X�`���̓ǂݍ���
    g_score_rank_C_Texture = InitTexture(L"asset\\texture\\score_texture\\score_c.png");
    g_score_rank_B_Texture = InitTexture(L"asset\\texture\\score_texture\\score_b.png");
    g_score_rank_A_Texture = InitTexture(L"asset\\texture\\score_texture\\score_a.png");
    g_score_rank_S_Texture = InitTexture(L"asset\\texture\\score_texture\\score_s.png");

    //�g�[�^�������N�̃e�N�X�`���̓ǂݍ��݁i�X�^���v�j
    g_score_rank_C_stamp_Texture = InitTexture(L"asset\\texture\\score_texture\\stamp03_c.png");
    g_score_rank_B_stamp_Texture = InitTexture(L"asset\\texture\\score_texture\\stamp03_b.png");
    g_score_rank_A_stamp_Texture = InitTexture(L"asset\\texture\\score_texture\\stamp03_a.png");
    g_score_rank_S_stamp_Texture = InitTexture(L"asset\\texture\\score_texture\\stamp03_s.png");

    //�S�Ẳ����~�߂�
    app_atomex_stop_player();

    //���U���g��BGM��������
    app_atomex_start(RESULT_BGM);

    //�X�R�A�̎擾
    //�R�C���̖������擾���āA�����N�ɔ��f===============================
    m_coin_count = Item_Coin_UI::GetNowCoinCount(); //�R�C���̖������擾
    int coin_score;

    //�R�C���̖����ɉ����ăX�R�A�����N�̃e�N�X�`���ƃ|�C���g��ݒ�
    if (m_coin_count >= 30)
    {
        coin_score_texture = g_score_rank_A_Texture;
        coin_score = 100;
    }
    else if (m_coin_count >= 20)
    {
        coin_score_texture = g_score_rank_B_Texture;
        coin_score = 75;
    }
    else {
        coin_score_texture = g_score_rank_C_Texture;
        coin_score = 50;
    }

    //�����x���擾���āA�����N�ɔ��f����==============================
    m_gokai_count = Gokai_UI::GetNowGokaiCount(); //�����x���擾
    int gokai_score;

    //�����x�ɉ����č��������N�̃e�N�X�`���ƃ|�C���g��ݒ�
    if (m_gokai_count >= 3000) {
        gokai_score_texture = g_score_rank_A_Texture;
        gokai_score = 100;
    }
    else if (m_gokai_count >= 2000)
    {
        gokai_score_texture = g_score_rank_B_Texture;
        gokai_score = 75;
    }
    else {
        gokai_score_texture = g_score_rank_C_Texture;
        gokai_score = 50;
    }


    //�{�X�̃N���A�^�C�����擾���āA�����N�ɔ��f===============================
    Boss_1_1& boss = Boss_1_1::GetInstance(); //�{�X�̃C���X�^���X���擾
    float boss_clear_time = boss.GetBossElapsedTime(); //�{�X�̃N���A�^�C�����擾
    boss_clear_time /= 60;
    m_boss_clear_time = boss_clear_time;
    int boss_score;

    //�N���A�^�C���ɉ����������N�ƃX�R�A�|�C���g�̐ݒ�
    if (m_boss_clear_time == 0)
    {
        clear_time_score_texture = g_score_rank_C_Texture;
        boss_score = 100;
    }
    else if (m_boss_clear_time >= 180.0f)
    { //3���ȏ�
        clear_time_score_texture = g_score_rank_C_Texture;
        boss_score = 50;
    }
    else if (m_boss_clear_time >= 120.0f)
    { //2���ȏ�
        clear_time_score_texture = g_score_rank_B_Texture;
        boss_score = 75;
    }
    else { //2������
        clear_time_score_texture = g_score_rank_A_Texture;
        boss_score = 100;
    }


    //�g�[�^���̃X�R�A�|�C���g���v�Z================================
    m_total_score_points = (coin_score+ gokai_score + boss_score);

    //�g�[�^���̃X�R�A�|�C���g�ɉ����ă����N��ݒ�
    if (m_total_score_points >= 300)
    { //300�ȏ�Ȃ�S�����N
        total_score_texture = g_score_rank_S_stamp_Texture;
    }
    else if (m_total_score_points >= 250)
    { //250�ȏ�Ȃ�A�����N
        total_score_texture = g_score_rank_A_stamp_Texture;
    }
    else if (m_total_score_points >= 200)
    { //200�ȏ�Ȃ�B�����N
        total_score_texture = g_score_rank_B_stamp_Texture;
    }
    else { //200�ȉ��Ȃ�C�����N
        total_score_texture = g_score_rank_C_stamp_Texture;
    }
}

void ResultScene::Update()
{
    //�R���g���[���[�̓��͂̎󂯎��
    ControllerState state = GetControllerInput();

    if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
    {
        SceneManager& sceneManager = SceneManager::GetInstance();
        sceneManager.ChangeScene(SCENE_STAGE_SELECT);
    }

    switch (m_state)
    {
    case STATE_RESULT_START:
        m_back_alpha += 0.01;
        if (m_back_alpha >= 1)
        {
            g_wait_cnt++;
            if (g_wait_cnt == g_wait)
            {
                g_wait_cnt = 0;
                m_state = STATE_RESULT_COIN;
            }
        }
        break;
    case STATE_RESULT_COIN:
        if (m_coin_pos.x >= m_coin_pos_finish.x)
        {
            m_coin_pos.x -= m_ui_move_speed;
            m_coin_alpha += 0.01 * m_ui_move_speed;
        }
        else if (m_coin_score_alpha <= 1)
        {
            m_coin_score_alpha += 0.01 * m_score_alpha_move_speed;
        }
        else
        {
            g_wait_cnt++;
            if (g_wait_cnt == g_wait)
            {
                g_wait_cnt = 0;
                m_state = STATE_RESULT_DYNAMIC;
            }
        }
        break;
    case STATE_RESULT_DYNAMIC:
        if (m_gokai_pos.x >= m_gokai_pos_finish.x)
        {
            m_gokai_pos.x -= m_ui_move_speed;
            m_gokai_alpha += 0.01 * m_ui_move_speed;
        }
        else if (m_gokai_score_alpha <= 1)
        {
            m_gokai_score_alpha += 0.01 * m_score_alpha_move_speed;
        }
        else
        {
            g_wait_cnt++;
            if (g_wait_cnt == g_wait)
            {
                g_wait_cnt = 0;
                m_state = STATE_RESULT_BOSS;
            }
        }
        break;
    case STATE_RESULT_BOSS:
        if (m_boss_pos.x >= m_boss_pos_finish.x)
        {
            m_boss_pos.x -= m_ui_move_speed;
            m_boss_alpha += 0.01 * m_ui_move_speed;
        }
        else if (m_boss_score_alpha <= 1)
        {
            m_boss_score_alpha += 0.01 * m_score_alpha_move_speed;
        }
        else
        {
            g_wait_cnt++;
            if (g_wait_cnt == g_wait)
            {
                g_wait_cnt = 0;
                m_state = STATE_RESULT_TOTAL;
            }
        }
        break;
    case STATE_RESULT_TOTAL:
        if (m_total_gage_filling <= 1)
        {
            m_total_gage_filling += 0.01 * m_gage_move_speed;
        }
        else if (m_total_score_alpha <= 1)
        {
            m_total_score_alpha += 0.01 * m_score_alpha_move_speed;
        }
        break;
    default: 
        break;
    }
}

void ResultScene::Draw()
{

    //�o�b�t�@�N���A
    Clear();

    //2D�`��Ȃ̂Ő[�x����
    SetDepthEnable(false);

    {//���U���g��ʂ̔w�i�̕`��
        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_GameScreen_Texture);

        DrawSpriteOld(
            XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
            0.0f,
            XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
        );


        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_result_Texture);

        DrawSpriteOld(
            XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
            0.0f,
            XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), m_back_alpha
        );
    }


    {//�R�C���̃����N��`��
        //�R�C���̊l��������\��
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);
        int num = m_coin_count;
        int cnt = 0;
        while (1)
        {
            DrawSpriteAnimOld(
                XMFLOAT2(m_coin_pos.x - (NUMBER_SIZE * cnt), m_coin_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE),
                10, 1, num % 10, m_coin_alpha
            );
            num /= 10;
            cnt++;
            if (num == 0)
            {
                break;
            }
        };

        //�V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &coin_score_texture);

        DrawSpriteOld(
            XMFLOAT2(600, 215), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
            0.0f,
            XMFLOAT2(RANK_SIZE, RANK_SIZE), m_coin_score_alpha
        );
    }

    {//�����x�̃����N��`��
        //�����x�̐��l��\��
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);
        int num = m_gokai_count;
        int cnt = 0;
        while (1)
        {
            DrawSpriteAnimOld(
                XMFLOAT2(m_gokai_pos.x - (NUMBER_SIZE * cnt), m_gokai_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE),
                10, 1, num % 10, m_gokai_alpha
            );
            num /= 10;
            cnt++;
            if (num == 0)
            {
                break;
            }
        };

        //�V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &gokai_score_texture);

        DrawSpriteOld(
            XMFLOAT2(600, 295), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
            0.0f,
            XMFLOAT2(RANK_SIZE, RANK_SIZE), m_gokai_score_alpha
        );
    }

    {//�{�X�̃N���A�^�C�������N��`��
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);
        int sec = m_boss_clear_time % 60;
        int cnt = 0;
        while (cnt!=2)
        {
            DrawSpriteAnimOld(
                XMFLOAT2(m_boss_pos.x - (NUMBER_SIZE * cnt), m_boss_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE),
                10, 1, sec % 10, m_boss_alpha
            );
            sec /= 10;
            cnt++;
        };

        {//�R�����ŋ�؂�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_colon_Texture);
            DrawSpriteOld(
                XMFLOAT2(m_boss_pos.x - (NUMBER_SIZE * cnt), m_boss_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE), m_boss_alpha
            );
            cnt++;
        }

        GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);
        int min = m_boss_clear_time / 60;
        while (1)
        {
            DrawSpriteAnimOld(
                XMFLOAT2(m_boss_pos.x - (NUMBER_SIZE * cnt), m_boss_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE),
                10, 1, min % 10, m_boss_alpha
            );
            min /= 10;
            cnt++;
            if (min == 0)
            {
                break;
            }
        };

        //�V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &clear_time_score_texture);

        DrawSpriteOld(
            XMFLOAT2(600, 385), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
            0.0f,
            XMFLOAT2(RANK_SIZE, RANK_SIZE), m_boss_score_alpha
        );
    }

    {//�g�[�^���X�R�A�̃Q�[�W�\��
        //�Q�[�W�̃t���[���\��
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_gage_frame_Texture);

        DrawSpriteOld(
            XMFLOAT2(370, 475), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
            0.0f,
            XMFLOAT2(480, 50), m_back_alpha
        );

        //�Q�[�W�\��
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_gage_Texture);
        float size = m_total_score_points / 300.0f;
        DrawSpriteOld(
            XMFLOAT2(130+(240.0f * size * m_total_gage_filling), 475), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
            0.0f,
            XMFLOAT2(480.0f * (size) * m_total_gage_filling, 30.0f)
        );

        //�Q�[�W�̃t���[���\��
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_gage_frame2_Texture);

        DrawSpriteOld(
            XMFLOAT2(370, 475), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
            0.0f,
            XMFLOAT2(480, 50), m_back_alpha
        );
    }

    {//�g�[�^���̃����N��`��
        //�V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &total_score_texture);

        DrawSpriteOld(
            XMFLOAT2(520, 560), //��U�}�W�b�N�i���o�[�ŊǗ��B��ŕς���
            0.0f,
            XMFLOAT2(TOTAL_RANK_SIZE, TOTAL_RANK_SIZE), m_total_score_alpha
        );
    }

    //�o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
    Present();

}

void ResultScene::Finalize()
{
    if (g_GameScreen_Texture) UnInitTexture(g_GameScreen_Texture);

    if (g_result_Texture) UnInitTexture(g_result_Texture);
    if (g_number_Texture) UnInitTexture(g_number_Texture);
    if (g_gage_Texture) UnInitTexture(g_gage_Texture);
    if (g_gage_frame_Texture) UnInitTexture(g_gage_frame_Texture);
    if (g_gage_frame2_Texture) UnInitTexture(g_gage_frame2_Texture);
    if (g_gage_points_Texture) UnInitTexture(g_gage_points_Texture);
    if (g_colon_Texture) UnInitTexture(g_colon_Texture);

    if (total_score_texture) UnInitTexture(total_score_texture);
    if (coin_score_texture) UnInitTexture(coin_score_texture);
    if (clear_time_score_texture) UnInitTexture(clear_time_score_texture);
    if (gokai_score_texture) UnInitTexture(gokai_score_texture);

    if (g_score_rank_C_stamp_Texture) UnInitTexture(g_score_rank_C_stamp_Texture);
    if (g_score_rank_B_stamp_Texture) UnInitTexture(g_score_rank_B_stamp_Texture);
    if (g_score_rank_A_stamp_Texture) UnInitTexture(g_score_rank_A_stamp_Texture);
    if (g_score_rank_S_stamp_Texture) UnInitTexture(g_score_rank_S_stamp_Texture);

    if (g_score_rank_C_Texture) UnInitTexture(g_score_rank_C_Texture);
    if (g_score_rank_B_Texture) UnInitTexture(g_score_rank_B_Texture);
    if (g_score_rank_A_Texture) UnInitTexture(g_score_rank_A_Texture);
    if (g_score_rank_S_Texture) UnInitTexture(g_score_rank_S_Texture);

}