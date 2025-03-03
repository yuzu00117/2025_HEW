//-----------------------------------------------------------------------------------------------------
// #name GamePause.cpp
// #description     �Q�[���̃|�[�Y���
// #make 2025/2/28�@���j�S
// #update 2025/2/28
// #comment �ǉ��E�C���\��
//                  
//                  
//----------------------------------------------------------------------------------------------------
#include "GamePause.h"
#include "texture.h"
#include "sprite.h"
#include "main.h"
#include "keyboard.h"
#include "Xinput_controller.h"
#include "player.h"
#include "game.h"
#include "scene.h"
#include "Item_Coin_UI.h"
#include "Gauge_UI.h"

//�O���[�o���ϐ�
static ID3D11ShaderResourceView* g_UnPause_words_Texture;				//�|�[�Y�����{�^���̃e�N�X�`��
static ID3D11ShaderResourceView* g_Respawn_SavePoint_words_Texture;     //���Ԓn�Ƀ��X�|�����邽�߂̃{�^���̃e�N�X�`��
static ID3D11ShaderResourceView* g_Respawn_InitalPoint_words_Texture;   //���X�|�����邽�߂̃{�^���̃e�N�X�`��
static ID3D11ShaderResourceView* g_SelectScene_words_Texture;           //�Z���N�g��ʂɖ߂�{�^���̃e�N�X�`��
static ID3D11ShaderResourceView* g_TitleScene_words_Texture;            //�^�C�g����ʂɖ߂�{�^���̃e�N�X�`��
static ID3D11ShaderResourceView* g_button_frame_Texture;				//�I������Ă��Ȃ��{�^���̘g�̃e�N�X�`��
static ID3D11ShaderResourceView* g_button_selected_frame_Texture;		//�I�����ꂽ�{�^���̘g�̃e�N�X�`��
static ID3D11ShaderResourceView* g_button_locked_frame_Texture;		    //�I���ł��Ȃ��{�^���̘g�̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseBackground_Texture;				//�|�[�Y��ʂ̔w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseSelectionBackground_Texture;	//�|�[�Y��ʂ̑I�����̔w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseSelectionHand_Texture;	        //�|�[�Y��ʂ̑I�������w����̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseUIRecordingBackground_Texture;	//�|�[�Y��ʂ̎擾���L�^�̔w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseUIRecording_JewelRing_Texture;	//�|�[�Y��ʂ̕�΂̃����O�̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseUIRecording_Jewel_Blue_Texture; //�|�[�Y��ʂ̐�΂̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseUIRecording_Jewel_Yellow_Texture; //�|�[�Y��ʂ̐�΂̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseUIRecording_Jewel_Red_Texture;  //�|�[�Y��ʂ̐�΂̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseUIRecording_CoinUI_Texture; 	//�|�[�Y��ʂ̃R�C��UI�̃e�N�X�`��
static ID3D11ShaderResourceView* g_PauseUIRecording_NumberUI_Texture; 	//�|�[�Y��ʂ̐���UI�̃e�N�X�`��

bool    Respawn_SavePoint = false;
int     Max_Coin = 0;
int     Current_Coin = 0;


//�{�^���̃T�C�Y
XMFLOAT2 g_button_scale[BUTTON_NUM] =
{
    {370.0f,60.0f},
    {370.0f,60.0f},
    {370.0f,60.0f},
    {370.0f,60.0f},
    {370.0f,60.0f}
};

float space = 100.0f;
//�{�^���̍��W
XMFLOAT2 g_button_position[BUTTON_NUM] =
{
    {320.0f, 220.0f},
    {320.0f, g_button_position[0].y + space},
    {320.0f, g_button_position[1].y + space},
    {320.0f, g_button_position[2].y + space},
    {320.0f, g_button_position[3].y + space}
};
//�I�����̔w�i�i���j
XMFLOAT2    g_selection_background_scale = { 650.0f, 870.0f };
XMFLOAT2    g_selection_background_position = { g_button_position[1].x, 330.0f};
//�I�������w����
XMFLOAT2    g_selection_hand_scale = { 50.0f, 50.0f };
XMFLOAT2    g_selection_hand_position = { g_button_position[0].x + (g_button_scale[0].x/2)*1.15f, g_button_position[0].y};
//�擾���̔w�i�i���j
XMFLOAT2    g_UIRecording_background_scale = { 650.0f, 420.0f };
XMFLOAT2    g_UIRecording_background_position = { 950.0f, 520.0f};
//��΂̃����O
XMFLOAT2    g_UIRecording_JewelRing_scale = { 260.0f, 200.0f };
XMFLOAT2    g_UIRecording_JewelRing_position = { g_UIRecording_background_position.x, g_UIRecording_background_position.y * 0.85f };
//��΁i0�F�@�@�P�F���F�@�@2�F�ԁj
XMFLOAT2 g_UIRecording_Jewel_scale[3] =
{
    {400.0f,800.0f},
    {400.0f,800.0f},
    {400.0f,800.0f}
};
XMFLOAT2 g_UIRecording_Jewel_position[3] =
{
    {g_UIRecording_JewelRing_position.x * 1.064f, g_UIRecording_JewelRing_position.y * 0.33f},
    {g_UIRecording_JewelRing_position.x * 1.069f, g_UIRecording_JewelRing_position.y * 0.33f},
    {g_UIRecording_JewelRing_position.x * 1.06f, g_UIRecording_JewelRing_position.y * 0.33f}
};
//�R�C���̃A�C�R��
XMFLOAT2    g_UIRecording_CoinUI_scale = { 120.0f, 120.0f };
XMFLOAT2    g_UIRecording_CoinUI_position = { g_UIRecording_background_position.x * 0.83f, g_UIRecording_background_position.y * 1.15f };
//�R�C���̐����\��
XMFLOAT2    g_UIRecording_NumberUI_scale = { 40.0f, 40.0f };
XMFLOAT2    g_UIRecording_NumberUI_Left_position = { g_UIRecording_background_position.x, g_UIRecording_background_position.y * 1.19f };
XMFLOAT2    g_UIRecording_NumberUI_Right_position = { g_UIRecording_background_position.x * 1.12f, g_UIRecording_background_position.y * 1.19f };

void GamePause::Initialize()
{
    g_UnPause_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_ResumeGame.png");
    g_Respawn_SavePoint_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_Restart_Checkpoint.png");
    g_Respawn_InitalPoint_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_RestartBeginning.png");
    g_SelectScene_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_ReturnStageSelect.png");
    g_TitleScene_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_ReturnTitle.png");
    g_button_frame_Texture = InitTexture(L"asset\\texture\\Pause_texture\\button_frame.png");
    g_button_selected_frame_Texture = InitTexture(L"asset\\texture\\Pause_texture\\button_selected_frame.png");
    g_button_locked_frame_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_RestartCheckpoint_NOcheckpoint.png");
    g_PauseBackground_Texture = InitTexture(L"asset\\texture\\Pause_texture\\PauseBackground.png");
    g_PauseSelectionBackground_Texture = InitTexture(L"asset\\texture\\Pause_texture\\pose_BG.png");
    g_PauseSelectionHand_Texture = InitTexture(L"asset\\texture\\Pause_texture\\stage_select_player.png");
    g_PauseUIRecordingBackground_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_pose_koin&houseki_BG.png");
    g_PauseUIRecording_JewelRing_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_houseiki_ringng.png");
    g_PauseUIRecording_Jewel_Blue_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\blue_jewel.png");
    g_PauseUIRecording_Jewel_Yellow_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\yellow_jewel.png");
    g_PauseUIRecording_Jewel_Red_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\red_jewel.png");
    g_PauseUIRecording_CoinUI_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_koin_akon.png");
    g_PauseUIRecording_NumberUI_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_number.png");

    Max_Coin = 0;
    Current_Coin = 0;
    Max_Coin = Item_Coin_UI::GetMaxCoinCount();
}

void GamePause::Finalize()
{
    if (g_UnPause_words_Texture) { UnInitTexture(g_UnPause_words_Texture); }
    if (g_Respawn_SavePoint_words_Texture) { UnInitTexture(g_Respawn_SavePoint_words_Texture); }
    if (g_Respawn_InitalPoint_words_Texture) { UnInitTexture(g_Respawn_InitalPoint_words_Texture); }
    if (g_SelectScene_words_Texture) { UnInitTexture(g_SelectScene_words_Texture); }
    if (g_TitleScene_words_Texture) { UnInitTexture(g_TitleScene_words_Texture); }
    if (g_button_frame_Texture) { UnInitTexture(g_button_frame_Texture); }
    if (g_button_selected_frame_Texture) { UnInitTexture(g_button_selected_frame_Texture); }
    if (g_button_locked_frame_Texture) { UnInitTexture(g_button_locked_frame_Texture); }
    if (g_PauseBackground_Texture) { UnInitTexture(g_PauseBackground_Texture); }
    if (g_PauseSelectionBackground_Texture) { UnInitTexture(g_PauseSelectionBackground_Texture); }
    if (g_PauseSelectionHand_Texture) { UnInitTexture(g_PauseSelectionHand_Texture); }
    if (g_PauseUIRecordingBackground_Texture) { UnInitTexture(g_PauseUIRecordingBackground_Texture); }
    if (g_PauseUIRecording_JewelRing_Texture) { UnInitTexture(g_PauseUIRecording_JewelRing_Texture); }
    if (g_PauseUIRecording_CoinUI_Texture) { UnInitTexture(g_PauseUIRecording_CoinUI_Texture); }
    if (g_PauseUIRecording_NumberUI_Texture) { UnInitTexture(g_PauseUIRecording_NumberUI_Texture); }
}

void GamePause::Update()
{
    Player& player = Player::GetInstance();
    if (player.GetPrevRegisteredSavePoint() != nullptr)
    {
        Respawn_SavePoint = true;
    }
    else
    {
        Respawn_SavePoint = false;
    }
    
    Current_Coin = Item_Coin_UI::GetNowCoinCount();

    // �R���g���[���[�̓��͂̎󂯎��
    ControllerState state = GetControllerInput();
    //���L�[�ŉ��I��
    if(state.leftStickY < 0 && key_flag.CountTime > 15.0f)
    {
        switch (m_button_selected)
        {
        case Button_UnPause:
            m_button_selected = Button_Respawn_SavePoint;
            if (!Respawn_SavePoint)
            {
                m_button_selected = Button_Respawn_InitalPoint;
            }
            break;
        case Button_Respawn_SavePoint:
            m_button_selected = Button_Respawn_InitalPoint;
            break;
        case Button_Respawn_InitalPoint:
            m_button_selected = Button_SelectScene;
            break;
        case Button_SelectScene:
            m_button_selected = Button_TitleScene;
            break;
        case Button_TitleScene:
            m_button_selected = Button_UnPause;
            break;
        case Button_NULL:
            m_button_selected = Button_UnPause;
            break;
        }
        key_flag.CountTime = 0.0f;
        g_selection_hand_position.y = g_button_position[m_button_selected].y + 20.0f;
    }
    //���L�[�ŏ�I��
    else if (state.leftStickY > 0 && key_flag.CountTime > 15.0f)
    {
        switch (m_button_selected)
        {
        case Button_UnPause:
            m_button_selected = Button_TitleScene;
            break;
        case Button_Respawn_SavePoint:
            m_button_selected = Button_UnPause;
            break;
        case Button_Respawn_InitalPoint:
            m_button_selected = Button_Respawn_SavePoint;
            if (!Respawn_SavePoint)
            {
                m_button_selected = Button_UnPause;
            }
            break;
        case Button_SelectScene:
            m_button_selected = Button_Respawn_InitalPoint;
            break;
        case Button_TitleScene:
            m_button_selected = Button_SelectScene;
            break;
        case Button_NULL:
            m_button_selected = Button_TitleScene;
            break;
        }
        key_flag.CountTime = 0.0f;
        g_selection_hand_position.y = g_button_position[m_button_selected].y + 20.0f;
    }

    key_flag.CountTime++;

    //�I���m��
    if (state.buttonA && !key_flag.ControllerButton_A)
    {
       SceneManager& sceneManager = SceneManager::GetInstance();
       Game& game = Game::GetInstance();

       game.SetCurrentGameState(GAME_STATE_RESPAWN_INITIAL);    //game�̕��̏����ɉe���Ȃ��̂ŁA�K���ő��v

       switch(m_button_selected)
       {
       case Button_UnPause:
          // game.SetCurrentGameState(GAME_STATE_RESPAWN_INITIAL);    //game�̕��̏����ɉe���Ȃ��̂ŁA�K���ő��v
           break;
       case Button_Respawn_SavePoint:
       {
           game.SetNextGameState(GAME_STATE_PAUSE_RESPAWN_SAVE_POINT);
           Player& player = Player::GetInstance();
           ItemSavePoint* registered_SavePoint = player.GetRegisteredSavePoint();
           //���̃X�e�[�W�ɓo�^�������Ԓn���Ȃ�������A�O�X�e�[�W�̓o�^�������Ԓn�ɍs��
           if (registered_SavePoint == nullptr)
           {
               registered_SavePoint = player.GetPrevRegisteredSavePoint();
               sceneManager.SetStageName(static_cast<STAGE_NAME>(registered_SavePoint->GetSavePoint_StageID()));
               player.RegisterSavePoint(registered_SavePoint);
           }
           else
           {
               sceneManager.SetStageName(static_cast<STAGE_NAME>(registered_SavePoint->GetSavePoint_StageID()));
           }
           sceneManager.Set_Chenge_Scene_flag(true);
       }
           break;
       case Button_Respawn_InitalPoint:
           game.SetNextGameState(GAME_STATE_PAUSE_RESPAWN_INITIAL);
           sceneManager.SetStageName(STAGE_1_1);
           sceneManager.Set_Chenge_Scene_flag(true);
           break;
       case Button_SelectScene:
           game.SetNextGameState(GAME_STATE_PAUSE_SELECT_SCENE);
           sceneManager.SetStageName(STAGE_SELECT);
           sceneManager.Set_Chenge_Scene_flag(true);
           break;
       case Button_TitleScene:
           game.SetNextGameState(GAME_STATE_PAUSE_TITLE);
           sceneManager.Set_Chenge_Scene_flag(true);
           break;
       }
       
       m_button_selected = Button_NULL;
    }

#ifndef _DEBUG
    //���L�[�ŉ��I��
    if (Keyboard_IsKeyDown(KK_DOWN) && key_flag.CountTime > 15.0f)
    {
        switch (m_button_selected)
        {
        case Button_UnPause:
            m_button_selected = Button_Respawn_SavePoint;
            break;
        case Button_Respawn_SavePoint:
            m_button_selected = Button_Respawn_InitalPoint;
            break;
        case Button_Respawn_InitalPoint:
            m_button_selected = Button_SelectScene;
            break;
        case Button_SelectScene:
            m_button_selected = Button_TitleScene;
            break;
        case Button_TitleScene:
            m_button_selected = Button_UnPause;
            break;
        }
        key_flag.CountTime = 0.0f;
        g_selection_hand_position.y = g_button_position[m_button_selected].y + 20.0f;
    }
    //���L�[�ŏ�I��
    else if (Keyboard_IsKeyDown(KK_UP) && key_flag.CountTime > 15.0f)
    {
        switch (m_button_selected)
        {
        case Button_UnPause:
            m_button_selected = Button_TitleScene;
            break;
        case Button_Respawn_SavePoint:
            m_button_selected = Button_UnPause;
            break;
        case Button_Respawn_InitalPoint:
            m_button_selected = Button_Respawn_SavePoint;
            break;
        case Button_SelectScene:
            m_button_selected = Button_Respawn_InitalPoint;
            break;
        case Button_TitleScene:
            m_button_selected = Button_SelectScene;
            break;
        }
        key_flag.CountTime = 0.0f;
        g_selection_hand_position.y = g_button_position[m_button_selected].y + 20.0f;
    }


    key_flag.CountTime++;

    //�I���m��
    if (Keyboard_IsKeyDown(KK_ENTER) && !key_flag.KeyboardButton_Enter)
    {
        SceneManager& sceneManager = SceneManager::GetInstance();
        Game& game = Game::GetInstance();

        game.SetCurrentGameState(GAME_STATE_RESPAWN_INITIAL);    //game�̕��̏����ɉe���Ȃ��̂ŁA�K���ő��v

        switch (m_button_selected)
        {
        case Button_UnPause:
            // game.SetCurrentGameState(GAME_STATE_RESPAWN_INITIAL);    //game�̕��̏����ɉe���Ȃ��̂ŁA�K���ő��v
            break;
        case Button_Respawn_SavePoint:
        {
            game.SetNextGameState(GAME_STATE_PAUSE_RESPAWN_SAVE_POINT);
            Player& player = Player::GetInstance();
            ItemSavePoint* registered_SavePoint = player.GetRegisteredSavePoint();
            //���̃X�e�[�W�ɓo�^�������Ԓn���Ȃ�������A�O�X�e�[�W�̓o�^�������Ԓn�ɍs��
            if (registered_SavePoint == nullptr)
            {
                registered_SavePoint = player.GetPrevRegisteredSavePoint();
                sceneManager.SetStageName(static_cast<STAGE_NAME>(registered_SavePoint->GetSavePoint_StageID()));
                player.RegisterSavePoint(registered_SavePoint);
            }
            else
            {
                sceneManager.SetStageName(static_cast<STAGE_NAME>(registered_SavePoint->GetSavePoint_StageID()));
            }
            sceneManager.Set_Chenge_Scene_flag(true);
        }
        break;
        case Button_Respawn_InitalPoint:
            game.SetNextGameState(GAME_STATE_PAUSE_RESPAWN_INITIAL);
            sceneManager.SetStageName(STAGE_1_1);
            sceneManager.Set_Chenge_Scene_flag(true);
            break;
        case Button_SelectScene:
            game.SetNextGameState(GAME_STATE_PAUSE_SELECT_SCENE);
            sceneManager.SetStageName(STAGE_SELECT);
            sceneManager.Set_Chenge_Scene_flag(true);
            break;
        case Button_TitleScene:
            game.SetNextGameState(GAME_STATE_PAUSE_TITLE);
            sceneManager.Set_Chenge_Scene_flag(true);
            break;
        }

        m_button_selected = Button_NULL;

    }

#endif // _DEBUG
}

void GamePause::Draw()
{
    //�|�[�Y��ʔw�i
    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseBackground_Texture);
    DrawSpriteOld(XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER), 0.0f, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), 0.7f);

    //�|�[�Y��ʃA�C�e���擾���̔w�i
    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecordingBackground_Texture);
    DrawSpriteOld(g_UIRecording_background_position, 0.0f, g_UIRecording_background_scale);
    
    //�|�[�Y��ʂ̕�΂̃����O
    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_JewelRing_Texture);
    DrawSpriteOld(g_UIRecording_JewelRing_position, 0.0f, g_UIRecording_JewelRing_scale);

    //�|�[�Y��ʂ̕�΂̃����O
    if (Gauge_UI::GetIfJewelCollected(BLUE))
    {
        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_Jewel_Blue_Texture);
        DrawSpriteOld(g_UIRecording_Jewel_position[0], 0.0f, g_UIRecording_Jewel_scale[0]);
    }
    if (Gauge_UI::GetIfJewelCollected(YELLOW))
    {
        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_Jewel_Yellow_Texture);
        DrawSpriteOld(g_UIRecording_Jewel_position[1], 0.0f, g_UIRecording_Jewel_scale[1]);
    }
    if (Gauge_UI::GetIfJewelCollected(RED))
    {
        // �V�F�[�_���\�[�X��ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_Jewel_Red_Texture);
        DrawSpriteOld(g_UIRecording_Jewel_position[2], 0.0f, g_UIRecording_Jewel_scale[2]);
    }

    //�|�[�Y��ʂ̃R�C��UI
    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_CoinUI_Texture);
    DrawSpriteOld(g_UIRecording_CoinUI_position, 0.0f, g_UIRecording_CoinUI_scale);

    //�|�[�Y��ʂ̐���UI
    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_NumberUI_Texture);
    int cnt = Current_Coin;
    // �����̕\�� ���݂̗�
    for (int i = 0; i < 2; i++)
    {
        DrawDividedSprite({ g_UIRecording_NumberUI_Left_position.x - (i * 45), g_UIRecording_NumberUI_Left_position.y }, 0.0f, g_UIRecording_NumberUI_scale, 10, 1, cnt);
        cnt /= 10;
    }
    //�@�E���̕\���@�ő吔
    int max_cnt = Max_Coin;
    for (int i = 0; i < 2; i++)
    {
        DrawDividedSprite({ g_UIRecording_NumberUI_Right_position.x - (i * 40), g_UIRecording_NumberUI_Right_position.y }, 0.0f, g_UIRecording_NumberUI_scale, 10, 1, max_cnt);
        max_cnt /= 10;
    }


    //----------------------------------------------------------------------------------------------------------------------------
    //�|�[�Y��ʑI�����̔w�i
    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseSelectionBackground_Texture);
    DrawSpriteOld(g_selection_background_position, 0.0f, g_selection_background_scale);

    //�I�����ꂽ���̃T�C�Y���� (����͔{��)
    XMFLOAT2 selected_size = { 1.0f,1.0f };

    for (int i = 0; i < BUTTON_NUM; i++)
    {
        if (m_button_selected == i)
        {
            selected_size = { 1.15f,1.15f };
        }
        switch (i)
        {
        case Button_UnPause:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_UnPause_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            break;
        case Button_Respawn_SavePoint:
            if (!Respawn_SavePoint)
            {
                // �V�F�[�_���\�[�X��ݒ�
                GetDeviceContext()->PSSetShaderResources(0, 1, &g_button_locked_frame_Texture);
                DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
            }
            else
            {
                // �V�F�[�_���\�[�X��ݒ�
                GetDeviceContext()->PSSetShaderResources(0, 1, &g_Respawn_SavePoint_words_Texture);
                DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            }
            break;        
        case Button_Respawn_InitalPoint:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Respawn_InitalPoint_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            break;
        case Button_SelectScene:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_SelectScene_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            break;
        case Button_TitleScene:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_TitleScene_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            break;
        }

        selected_size = { 1.0f, 1.0f };

        //�I�������w����̕`��
        if (m_button_selected == i)
        {
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseSelectionHand_Texture);
            DrawSpriteOld(g_selection_hand_position, 0.0f, g_selection_hand_scale);
        }

    }

}
