//-----------------------------------------------------------------------------------------------------
// #name camera_shake
// #description カメラシェイク
// #make 2025/01/17　　永野義也
// #update 2025/01/17
// #comment 追加・修正予定
//          
// 
//----------------------------------------------------------------------------------------------------

#include"display.h"
#include"camera_shake.h"
#include"tool.h"
#include"player_position.h"
#include"player.h"
#include"include/box2d/box2d.h"
#include <algorithm> // 追加

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
	

		//カメラシェイク終了
		if (camera_shake_frame <= 0)
		{
			//リセット処理
			camera_shake_flag = false;
			camera_shake_height = 0;
			camera_shake_width = 0;

			display::SetDisplayDefault();

		}
	}

    // **フレーム数でカメラリセットを管理**
    const int RETURN_FRAME_THRESHOLD = 30; // 30フレーム（例: 60FPSなら0.5秒）
    const float THRESHOLD = 0.03f;  // これ以下の移動は無視
    const float MAX_OFFSET_X = 100.0f;  // カメラのX方向の最大ズレ量
    const float MAX_OFFSET_Y = 25.0f;   // カメラのY方向の最大ズレ量
    const float CAMERA_MOVE_SPEED_X = 4.0f; // X軸の移動速度
    const float CAMERA_MOVE_SPEED_Y = 2.0f; // Y軸の移動速度

    static int lastMoveFrame = 0;    // 最後に操作があったフレーム
    static int currentFrame = 0;     // 現在のフレーム数（毎フレーム増加）

    // **フレームを更新**
    currentFrame++;

    // プレイヤーの位置を取得
    float playerX = PlayerPosition::GetPlayerPosition().x;
    float oldX = old_olayer_pos.x;
    float playerY = PlayerPosition::GetPlayerPosition().y;
    float oldY = old_olayer_pos.y;

    // 変化が小さい場合は処理しない
    bool moved = false;

    // **X軸（横移動）の処理**
    if (std::abs(playerX - oldX) >= THRESHOLD) {
        float displayWidth = display::GetDisplayWidth();
        float diffX = displayWidth - DEFAULT_DISPLAY_WIDTH;

        // **カメラのX方向の移動を許可**
        float moveX = 0.0f;
        if (playerX < oldX) { // 左へ移動
            moveX = clamp(CAMERA_MOVE_SPEED_X, -MAX_OFFSET_X - diffX, MAX_OFFSET_X - diffX);
        }
        else if (playerX > oldX) { // 右へ移動
            moveX = clamp(-CAMERA_MOVE_SPEED_X, -MAX_OFFSET_X - diffX, MAX_OFFSET_X - diffX);
        }

        if (moveX != 0.0f) {
            display::AddDisplayWidth(moveX);
            moved = true;
        }
    }

    // **Y軸（縦移動）の処理**
    if (std::abs(playerY - oldY) >= THRESHOLD) {
        float displayHeight = display::GetDisplayHeight();
        float diffY = displayHeight - DEFAULT_DISPLAY_HEIGHT;

        // **カメラのY方向の移動を許可**
        float moveY = 0.0f;
        if (playerY > oldY) { // 上へ移動
            moveY = clamp(CAMERA_MOVE_SPEED_Y, -MAX_OFFSET_Y - diffY, MAX_OFFSET_Y - diffY);
        }
        else if (playerY < oldY) { // 下へ移動
            moveY = clamp(-CAMERA_MOVE_SPEED_Y, -MAX_OFFSET_Y - diffY, MAX_OFFSET_Y - diffY);
        }

        if (moveY != 0.0f) {
            display::AddDisplayHeight(moveY);
            moved = true;
        }
    }

    // **プレイヤーが移動した場合、リセットフレームを更新**
    if (moved) {
        lastMoveFrame = currentFrame;
    }

    // **一定フレーム間操作がなければカメラをリセット**
    if (currentFrame - lastMoveFrame > RETURN_FRAME_THRESHOLD) {
        float displayWidth = display::GetDisplayWidth();
        float displayHeight = display::GetDisplayHeight();

        // **カメラをより速く元の位置に戻す**
        float resetSpeed = 7.0f; // 少し速く戻るように調整

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
