//-----------------------------------------------------------------------------------------------------
// #name camera_shake
// #description �J�����V�F�C�N
// #make 2025/01/17�@�@�i��`��
// #update 2025/01/17
// #comment �ǉ��E�C���\��
//          
// 
//----------------------------------------------------------------------------------------------------

#include"display.h"
#include"camera_shake.h"
#include"tool.h"
#include"player_position.h"
#include"player.h"
#include"include/box2d/box2d.h"
#include <algorithm> // �ǉ�

template<typename T>
T clamp(T value, T min, T max) {
    return std::max(min, std::min(value, max));
}

bool CameraShake::camera_shake_minus = false;
bool CameraShake::camera_shake_flag = false;
int CameraShake::camera_shake_frame = 0;
int CameraShake::camera_shake_height = 0;
int CameraShake::camera_shake_width = 0;
b2Vec2 CameraShake::old_olayer_pos = { 0.0f,0.0f };


void CameraShake::Update()
{
    // �J�����̃I�t�Z�b�g�ϐ�
    static float moveOffsetX = 0.0f;
    static float moveOffsetY = 0.0f;
    static float shakeOffsetX = 0.0f;
    static float shakeOffsetY = 0.0f;

    // **�J�����V�F�C�N�̏���**
    if (camera_shake_flag)
    {
        camera_shake_frame--;

        float random = GetRandomInt(5, 10) * 0.1f; // 0.5 ~ 1.0 �̃����_���l

        if (camera_shake_frame % 4 == 0) // 4�t���[�����ƂɐU��
        {
            if (camera_shake_minus)
            {
                shakeOffsetX = random * camera_shake_width * -1;
                shakeOffsetY = random * camera_shake_height * -1;
                camera_shake_minus = false;
            }
            else
            {
                shakeOffsetX = random * camera_shake_width;
                shakeOffsetY = random * camera_shake_height;
                camera_shake_minus = true;
            }
        }

        // �J�����V�F�C�N�I������
        if (camera_shake_frame <= 0)
        {
            camera_shake_flag = false;
            camera_shake_height = 0;
            camera_shake_width = 0;
            shakeOffsetX = 0.0f;
            shakeOffsetY = 0.0f;

            display::SetDisplayDefault();
        }
    }

    // **�t���[�����ŃJ�������Z�b�g���Ǘ�**
    const int RETURN_FRAME_THRESHOLD = 30;
    const float THRESHOLD = 0.03f;
    const float MAX_OFFSET_X = 100.0f;
    const float MAX_OFFSET_Y = 50.0f;


    // X����Y���̃��Z�b�g�J�n�܂ł̃t���[�����ʂɐݒ�
    const int RETURN_FRAME_THRESHOLD_X = 30; // X����30�t���[����Ƀ��Z�b�g
    const int RETURN_FRAME_THRESHOLD_Y = 50; // Y����50�t���[����Ƀ��Z�b�g

    // X����Y���̃��Z�b�g���x���ʂɐݒ�
    const float RESET_SPEED_X = 2.0f;
    const float RESET_SPEED_Y = 2.0f;

    // X���̃J�����ړ����x
    const float CAMERA_MOVE_SPEED_X = 4.0f;

    // Y���̃J�����ړ����x�i��ړ��Ɖ��ړ��ŕ�����j
    const float CAMERA_MOVE_SPEED_Y_UP = 5.0f;   // ��ֈړ����̃X�s�[�h�i�����j
    const float CAMERA_MOVE_SPEED_Y_DOWN = 4.0f; // ���ֈړ����̃X�s�[�h�i�ʏ�j

    // �Ō�ɓ��������t���[�����ʂɋL�^
    static int lastMoveFrameX = 0;
    static int lastMoveFrameY = 0;
    static int currentFrame = 0;

    // �t���[�����X�V
    currentFrame++;

    // �v���C���[�̈ʒu���擾
    float playerX = PlayerPosition::GetPlayerPosition().x;
    float oldX = old_olayer_pos.x;
    float playerY = PlayerPosition::GetPlayerPosition().y;
    float oldY = old_olayer_pos.y;

    bool movedX = false;
    bool movedY = false;

    // X���̏���
    if (std::abs(playerX - oldX) >= THRESHOLD)
    {
        float diffX = moveOffsetX;
        float moveX = 0.0f;

        if (playerX > oldX) // �v���C���[���E�Ɉړ�
            moveX = clamp(-CAMERA_MOVE_SPEED_X, -MAX_OFFSET_X - diffX, MAX_OFFSET_X - diffX); // �J�����͍��Ɉړ�
        else if (playerX < oldX) // �v���C���[�����Ɉړ�
            moveX = clamp(CAMERA_MOVE_SPEED_X, -MAX_OFFSET_X - diffX, MAX_OFFSET_X - diffX);  // �J�����͉E�Ɉړ�

        if (moveX != 0.0f)
        {
            moveOffsetX += moveX;
            movedX = true;
        }
    }

    // Y���̏����i��ړ��Ɖ��ړ��ŃX�s�[�h��ύX�j
    if (std::abs(playerY - oldY) >= THRESHOLD)
    {
        float diffY = moveOffsetY;
        float moveY = 0.0f;

        if (playerY < oldY) // �v���C���[����Ɉړ��i�J��������ֈړ��j
            moveY = clamp(-CAMERA_MOVE_SPEED_Y_UP, -MAX_OFFSET_Y - diffY, MAX_OFFSET_Y - diffY);
        else if (playerY > oldY) // �v���C���[�����Ɉړ��i�J���������ֈړ��j
            moveY = clamp(CAMERA_MOVE_SPEED_Y_DOWN, -MAX_OFFSET_Y - diffY, MAX_OFFSET_Y - diffY);

        if (moveY != 0.0f)
        {
            moveOffsetY += moveY;
            movedY = true;
        }
    }

    // X���̈ړ����������ꍇ�A���Z�b�g�t���[�����X�V
    if (movedX)
        lastMoveFrameX = currentFrame;

    // Y���̈ړ����������ꍇ�A���Z�b�g�t���[�����X�V
    if (movedY)
        lastMoveFrameY = currentFrame;

    // ��莞�ԑ��삪�Ȃ��ꍇ�A�J���������Z�b�g
    if (currentFrame - lastMoveFrameX > RETURN_FRAME_THRESHOLD || currentFrame - lastMoveFrameY > RETURN_FRAME_THRESHOLD)
    {
        if (moveOffsetX != 0.0f)
        {
            float moveX = (0.0f - moveOffsetX) * 0.15f;
            moveOffsetX += (std::abs(moveX) < RESET_SPEED_X) ? moveX : ((moveX > 0) ? RESET_SPEED_X : -RESET_SPEED_X);
        }

        if (moveOffsetY != 0.0f)
        {
            float moveY = (0.0f - moveOffsetY) * 0.15f;
            moveOffsetY += (std::abs(moveY) < RESET_SPEED_Y) ? moveY : ((moveY > 0) ? RESET_SPEED_Y : -RESET_SPEED_Y);
        }
    }


    // �J�����̍ŏI�K�p
    display::SetDisplayWidth(DEFAULT_DISPLAY_WIDTH + moveOffsetX + shakeOffsetX);
    display::SetDisplayHeight(DEFAULT_DISPLAY_HEIGHT + moveOffsetY + shakeOffsetY);


    // **�O�t���[���̃v���C���[�ʒu���X�V**
    old_olayer_pos = PlayerPosition::GetPlayerPosition();
}