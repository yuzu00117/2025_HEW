
#ifndef XINPUT_H
#define XINPUT_H

#include <Windows.h>

struct ControllerState {
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

// �R���g���[���[�̓��͏�Ԃ��擾����֐�
ControllerState GetControllerInput();

// ���͏�Ԃ�\������֐�
void PrintControllerState(const ControllerState& state);

#endif // XINPUT_H