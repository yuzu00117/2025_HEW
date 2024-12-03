//-----------------------------------------------------------------------------------------------------
// #name Xinput_controller.h
// #description Xboxのコントローラーの入力
// #make 2024/11/22　永野義也
// #update 2024/11/22
// #comment 追加・修正予定
//          ・特になしと思う　ジャイロしたいならここかな？
//----------------------------------------------------------------------------------------------------
#ifndef XINPUT_H
#define XINPUT_H

#include <Windows.h>

struct ControllerState {//受けとる入力の種類
    bool isConnected;
    int leftStickX;
    int leftStickY;
    int rightStickX;
    int rightStickY;
    int leftTrigger;
    int rightTrigger;
    bool buttonA;
    bool buttonB;
    bool buttonX;
    bool buttonY;
    bool dpadUp;
    bool dpadDown;
    bool dpadLeft;
    bool dpadRight;
    bool leftShoulder;
    bool rightShoulder;
    bool start;
    bool back;
    bool leftThumb;
    bool rightThumb;
};

// コントローラーの入力状態を取得する関数
ControllerState GetControllerInput();

// 入力状態を表示する関数
void PrintControllerState(const ControllerState& state);

#endif // XINPUT_H