#include"Xinput_controller.h"
#include <iostream>
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

ControllerState GetControllerInput() {
    ControllerState controllerState = {};
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    // コントローラーの状態を取得（コントローラー番号0、複数コントローラーの場合0〜3で指定）
    DWORD result = XInputGetState(0, &state);

    if (result == ERROR_SUCCESS) {
        controllerState.isConnected = true;

        // 左スティックのX, Y軸値
        controllerState.leftStickX = state.Gamepad.sThumbLX;
        controllerState.leftStickY = state.Gamepad.sThumbLY;

        // 右スティックのX, Y軸値
        controllerState.rightStickX = state.Gamepad.sThumbRX;
        controllerState.rightStickY = state.Gamepad.sThumbRY;

        // トリガーの値
        controllerState.leftTrigger = state.Gamepad.bLeftTrigger;
        controllerState.rightTrigger = state.Gamepad.bRightTrigger;

        // ボタンの状態
        controllerState.buttonA = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
        controllerState.buttonB = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
        controllerState.buttonX = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
        controllerState.buttonY = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
        controllerState.dpadUp = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
        controllerState.dpadDown = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
        controllerState.dpadLeft = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
        controllerState.dpadRight = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
        controllerState.leftShoulder = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
        controllerState.rightShoulder = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
        controllerState.start = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
        controllerState.back = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
        controllerState.leftThumb = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
        controllerState.rightThumb = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
    }
    else {
        controllerState.isConnected = false;
    }

    return controllerState;
}

void PrintControllerState(const ControllerState& state) {
    if (!state.isConnected) {
        std::cout << "Controller is not connected." << std::endl;
        return;
    }

    std::cout << "Left Stick X: " << state.leftStickX << " Y: " << state.leftStickY << std::endl;
    std::cout << "Right Stick X: " << state.rightStickX << " Y: " << state.rightStickY << std::endl;
    std::cout << "Left Trigger: " << state.leftTrigger << " Right Trigger: " << state.rightTrigger << std::endl;
    if (state.buttonA) std::cout << "A Button Pressed" << std::endl;
    if (state.buttonB) std::cout << "B Button Pressed" << std::endl;
    if (state.buttonX) std::cout << "X Button Pressed" << std::endl;
    if (state.buttonY) std::cout << "Y Button Pressed" << std::endl;
    if (state.dpadUp) std::cout << "D-Pad Up Pressed" << std::endl;
    if (state.dpadDown) std::cout << "D-Pad Down Pressed" << std::endl;
    if (state.dpadLeft) std::cout << "D-Pad Left Pressed" << std::endl;
    if (state.dpadRight) std::cout << "D-Pad Right Pressed" << std::endl;
    if (state.leftShoulder) std::cout << "Left Shoulder Button Pressed" << std::endl;
    if (state.rightShoulder) std::cout << "Right Shoulder Button Pressed" << std::endl;
    if (state.start) std::cout << "Start Button Pressed" << std::endl;
    if (state.back) std::cout << "Back Button Pressed" << std::endl;
    if (state.leftThumb) std::cout << "Left Thumbstick Pressed" << std::endl;
    if (state.rightThumb) std::cout << "Right Thumbstick Pressed" << std::endl;
}