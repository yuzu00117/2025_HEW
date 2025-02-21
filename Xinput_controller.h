//-----------------------------------------------------------------------------------------------------
// #name Xinput_controller.h
// #description Xbox�̃R���g���[���[�̓���
// #make 2024/11/22�@�i��`��
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E���ɂȂ��Ǝv���@�W���C���������Ȃ炱�����ȁH
//----------------------------------------------------------------------------------------------------
#ifndef XINPUT_H
#define XINPUT_H

#include <Windows.h>

struct ControllerState {//�󂯂Ƃ���͂̎��
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


#include <Xinput.h>

class VibrationController {
public:
    // �U�����J�n�i�t���[�����w��j
    static void StartVibration(float leftMotor, float rightMotor, int frames);

    // �U���𑦒�~
    static void StopVibration();

    // �t���[�����Ƃ̍X�V�i���t���[����ɐU����~�j
    static void UpdateVibration();

private:
    static float leftMotorSpeed;
    static float rightMotorSpeed;
    static int remainingFrames;
    static bool isActive;

    static void ApplyVibration();
};



#endif // XINPUT_H