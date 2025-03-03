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

bool    Respawn_SavePoint = false;


//�{�^���̃T�C�Y
XMFLOAT2 g_button_scale[BUTTON_NUM] =
{
    {300.0f,100.0f},
    {300.0f,100.0f},
    {300.0f,100.0f},
    {300.0f,100.0f},
    {300.0f,100.0f}
};

//�{�^���̍��W
XMFLOAT2 g_button_position[BUTTON_NUM] =
{
    {SCREEN_XCENTER,SCREEN_HEIGHT / BUTTON_NUM - g_button_scale[0].y / 2},
    {SCREEN_XCENTER, SCREEN_HEIGHT / BUTTON_NUM * 2 - g_button_scale[1].y / 2},
    {SCREEN_XCENTER, SCREEN_HEIGHT / BUTTON_NUM * 3 - g_button_scale[2].y / 2},
    {SCREEN_XCENTER, SCREEN_HEIGHT / BUTTON_NUM * 4 - g_button_scale[3].y / 2},
    {SCREEN_XCENTER, SCREEN_HEIGHT - g_button_scale[2].y / 2}
};

XMFLOAT2    g_selection_background_scale = { g_button_scale[0].x + 200.0f, (g_button_position[BUTTON_NUM - 1].y + g_button_scale[BUTTON_NUM - 1].y / 2) - (g_button_position[0].y - g_button_scale[0].y / 2) + 100.0f };
XMFLOAT2    g_selection_background_position = { g_button_position[0].x, SCREEN_HEIGHT/2};


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
        }
        key_flag.CountTime = 0.0f;
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
        }
        key_flag.CountTime = 0.0f;
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
    }


    key_flag.CountTime++;

    //�I���m��
    if (Keyboard_IsKeyDown(KK_ENTER) && !key_flag.KeyboardButton_Enter)
    {

    }

#endif // _DEBUG
}

void GamePause::Draw()
{
    //�|�[�Y��ʔw�i
    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseBackground_Texture);
    DrawSpriteOld(XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER), 0.0f, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), 0.7f);

    //�|�[�Y��ʑI�����̔w�i
    // �V�F�[�_���\�[�X��ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseSelectionBackground_Texture);
    DrawSpriteOld(g_selection_background_position, 0.0f, g_selection_background_scale);

    for (int i = 0; i < BUTTON_NUM; i++)
    {
        //if (m_button_selected == i)
        //{
        //    //�|�[�Y��ʂ̃{�^���g�i�I�������j
        //    // �V�F�[�_���\�[�X��ݒ�
        //    GetDeviceContext()->PSSetShaderResources(0, 1, &g_button_selected_frame_Texture);
        //    DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
        //}
        //else
        //{
        //    //�|�[�Y��ʂ̃{�^���g�i���I���j
        //    // �V�F�[�_���\�[�X��ݒ�
        //    GetDeviceContext()->PSSetShaderResources(0, 1, &g_button_frame_Texture);
        //    DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
        //}

        switch (i)
        {
        case Button_UnPause:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_UnPause_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
            break;
        case Button_Respawn_SavePoint:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Respawn_SavePoint_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
            if (!Respawn_SavePoint)
            {
                GetDeviceContext()->PSSetShaderResources(0, 1, &g_button_locked_frame_Texture);
                DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
            }
            break;        
        case Button_Respawn_InitalPoint:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Respawn_InitalPoint_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
            break;
        case Button_SelectScene:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_SelectScene_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
            break;
        case Button_TitleScene:
            // �V�F�[�_���\�[�X��ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_TitleScene_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
            break;
        }
    }

}
