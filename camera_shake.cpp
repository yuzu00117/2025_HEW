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
	if (camera_shake_flag == true)
	{
		camera_shake_frame--;

		float random = GetRandomInt(5, 10);
		random = random * 0.1;
		
		if (camera_shake_frame % 4 == 0)
		{
			if (camera_shake_minus)
			{
				display::AddDisplayHeight(random * camera_shake_height * -1);
				display::AddDisplayWidth(random * camera_shake_width * -1);
				camera_shake_minus = false;
			}
			else
			{
				display::AddDisplayHeight(random * camera_shake_height);
				display::AddDisplayWidth(random * camera_shake_width);
				camera_shake_minus = true;
			}
		}
	

		//�J�����V�F�C�N�I��
		if (camera_shake_frame <= 0)
		{
			//���Z�b�g����
			camera_shake_flag = false;
			camera_shake_height = 0;
			camera_shake_width = 0;

			display::SetDisplayDefault();

		}
	}

    // **�t���[�����ŃJ�������Z�b�g���Ǘ�**
    const int RETURN_FRAME_THRESHOLD = 30; // 30�t���[���i��: 60FPS�Ȃ�0.5�b�j
    const float THRESHOLD = 0.03f;  // ����ȉ��̈ړ��͖���
    const float MAX_OFFSET_X = 100.0f;  // �J������X�����̍ő�Y����
    const float MAX_OFFSET_Y = 25.0f;   // �J������Y�����̍ő�Y����
    const float CAMERA_MOVE_SPEED_X = 4.0f; // X���̈ړ����x
    const float CAMERA_MOVE_SPEED_Y = 2.0f; // Y���̈ړ����x

    static int lastMoveFrame = 0;    // �Ō�ɑ��삪�������t���[��
    static int currentFrame = 0;     // ���݂̃t���[�����i���t���[�������j

    // **�t���[�����X�V**
    currentFrame++;

    // �v���C���[�̈ʒu���擾
    float playerX = PlayerPosition::GetPlayerPosition().x;
    float oldX = old_olayer_pos.x;
    float playerY = PlayerPosition::GetPlayerPosition().y;
    float oldY = old_olayer_pos.y;

    // �ω����������ꍇ�͏������Ȃ�
    bool moved = false;

    // **X���i���ړ��j�̏���**
    if (std::abs(playerX - oldX) >= THRESHOLD) {
        float displayWidth = display::GetDisplayWidth();
        float diffX = displayWidth - DEFAULT_DISPLAY_WIDTH;

        // **�J������X�����̈ړ�������**
        float moveX = 0.0f;
        if (playerX < oldX) { // ���ֈړ�
            moveX = clamp(CAMERA_MOVE_SPEED_X, -MAX_OFFSET_X - diffX, MAX_OFFSET_X - diffX);
        }
        else if (playerX > oldX) { // �E�ֈړ�
            moveX = clamp(-CAMERA_MOVE_SPEED_X, -MAX_OFFSET_X - diffX, MAX_OFFSET_X - diffX);
        }

        if (moveX != 0.0f) {
            display::AddDisplayWidth(moveX);
            moved = true;
        }
    }

    // **Y���i�c�ړ��j�̏���**
    if (std::abs(playerY - oldY) >= THRESHOLD) {
        float displayHeight = display::GetDisplayHeight();
        float diffY = displayHeight - DEFAULT_DISPLAY_HEIGHT;

        // **�J������Y�����̈ړ�������**
        float moveY = 0.0f;
        if (playerY > oldY) { // ��ֈړ�
            moveY = clamp(CAMERA_MOVE_SPEED_Y, -MAX_OFFSET_Y - diffY, MAX_OFFSET_Y - diffY);
        }
        else if (playerY < oldY) { // ���ֈړ�
            moveY = clamp(-CAMERA_MOVE_SPEED_Y, -MAX_OFFSET_Y - diffY, MAX_OFFSET_Y - diffY);
        }

        if (moveY != 0.0f) {
            display::AddDisplayHeight(moveY);
            moved = true;
        }
    }

    // **�v���C���[���ړ������ꍇ�A���Z�b�g�t���[�����X�V**
    if (moved) {
        lastMoveFrame = currentFrame;
    }

    // **���t���[���ԑ��삪�Ȃ���΃J���������Z�b�g**
    if (currentFrame - lastMoveFrame > RETURN_FRAME_THRESHOLD) {
        float displayWidth = display::GetDisplayWidth();
        float displayHeight = display::GetDisplayHeight();

        // **�J��������葬�����̈ʒu�ɖ߂�**
        float resetSpeed = 7.0f; // ���������߂�悤�ɒ���

        if (displayWidth != DEFAULT_DISPLAY_WIDTH) {
            float moveX = (DEFAULT_DISPLAY_WIDTH - displayWidth) * 0.15f;
            display::AddDisplayWidth((std::abs(moveX) < resetSpeed) ? moveX : ((moveX > 0) ? resetSpeed : -resetSpeed));
        }

        if (displayHeight != DEFAULT_DISPLAY_HEIGHT) {
            float moveY = (DEFAULT_DISPLAY_HEIGHT - displayHeight) * 0.15f;
            display::AddDisplayHeight((std::abs(moveY) < resetSpeed) ? moveY : ((moveY > 0) ? resetSpeed : -resetSpeed));
        }
    }
    Player& player = Player::GetInstance();
    b2Vec2 newPosition = player.GetPlayerBody()->GetPosition();
    player.GetSensorBody()->SetLinearVelocity((newPosition - player.GetSensorBody()->GetPosition()));
    old_olayer_pos = PlayerPosition::GetPlayerPosition();
}
