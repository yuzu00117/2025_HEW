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
    // カメラのオフセット変数
    static float moveOffsetX = 0.0f;
    static float moveOffsetY = 0.0f;
    static float shakeOffsetX = 0.0f;
    static float shakeOffsetY = 0.0f;

    // **カメラシェイクの処理**
    if (camera_shake_flag)
    {
        camera_shake_frame--;

        float random = GetRandomInt(5, 10) * 0.1f; // 0.5 ~ 1.0 のランダム値

        if (camera_shake_frame % 4 == 0) // 4フレームごとに振動
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

        // カメラシェイク終了処理
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

    // **フレーム数でカメラリセットを管理**
    const int RETURN_FRAME_THRESHOLD = 30;
    const float THRESHOLD = 0.03f;
    const float MAX_OFFSET_X = 100.0f;
    const float MAX_OFFSET_Y = 50.0f;


    // X軸とY軸のリセット開始までのフレームを個別に設定
    const int RETURN_FRAME_THRESHOLD_X = 30; // X軸は30フレーム後にリセット
    const int RETURN_FRAME_THRESHOLD_Y = 50; // Y軸は50フレーム後にリセット

    // X軸とY軸のリセット速度を個別に設定
    const float RESET_SPEED_X = 2.0f;
    const float RESET_SPEED_Y = 2.0f;

    // X軸のカメラ移動速度
    const float CAMERA_MOVE_SPEED_X = 4.0f;

    // Y軸のカメラ移動速度（上移動と下移動で分ける）
    const float CAMERA_MOVE_SPEED_Y_UP = 5.0f;   // 上へ移動時のスピード（強調）
    const float CAMERA_MOVE_SPEED_Y_DOWN = 4.0f; // 下へ移動時のスピード（通常）

    // 最後に動かしたフレームを個別に記録
    static int lastMoveFrameX = 0;
    static int lastMoveFrameY = 0;
    static int currentFrame = 0;

    // フレームを更新
    currentFrame++;

    // プレイヤーの位置を取得
    float playerX = PlayerPosition::GetPlayerPosition().x;
    float oldX = old_olayer_pos.x;
    float playerY = PlayerPosition::GetPlayerPosition().y;
    float oldY = old_olayer_pos.y;

    bool movedX = false;
    bool movedY = false;

    // X軸の処理
    if (std::abs(playerX - oldX) >= THRESHOLD)
    {
        float diffX = moveOffsetX;
        float moveX = 0.0f;

        if (playerX > oldX) // プレイヤーが右に移動
            moveX = clamp(-CAMERA_MOVE_SPEED_X, -MAX_OFFSET_X - diffX, MAX_OFFSET_X - diffX); // カメラは左に移動
        else if (playerX < oldX) // プレイヤーが左に移動
            moveX = clamp(CAMERA_MOVE_SPEED_X, -MAX_OFFSET_X - diffX, MAX_OFFSET_X - diffX);  // カメラは右に移動

        if (moveX != 0.0f)
        {
            moveOffsetX += moveX;
            movedX = true;
        }
    }

    // Y軸の処理（上移動と下移動でスピードを変更）
    if (std::abs(playerY - oldY) >= THRESHOLD)
    {
        float diffY = moveOffsetY;
        float moveY = 0.0f;

        if (playerY < oldY) // プレイヤーが上に移動（カメラも上へ移動）
            moveY = clamp(-CAMERA_MOVE_SPEED_Y_UP, -MAX_OFFSET_Y - diffY, MAX_OFFSET_Y - diffY);
        else if (playerY > oldY) // プレイヤーが下に移動（カメラも下へ移動）
            moveY = clamp(CAMERA_MOVE_SPEED_Y_DOWN, -MAX_OFFSET_Y - diffY, MAX_OFFSET_Y - diffY);

        if (moveY != 0.0f)
        {
            moveOffsetY += moveY;
            movedY = true;
        }
    }

    // X軸の移動があった場合、リセットフレームを更新
    if (movedX)
        lastMoveFrameX = currentFrame;

    // Y軸の移動があった場合、リセットフレームを更新
    if (movedY)
        lastMoveFrameY = currentFrame;

    // 一定時間操作がない場合、カメラをリセット
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


    // カメラの最終適用
    display::SetDisplayWidth(DEFAULT_DISPLAY_WIDTH + moveOffsetX + shakeOffsetX);
    display::SetDisplayHeight(DEFAULT_DISPLAY_HEIGHT + moveOffsetY + shakeOffsetY);


    // **前フレームのプレイヤー位置を更新**
    old_olayer_pos = PlayerPosition::GetPlayerPosition();
}