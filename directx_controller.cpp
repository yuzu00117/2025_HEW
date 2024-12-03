

//-----------------------------------------------------------------------------------------------------
// #name directx_controler.cpp
// #description game
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
//          �E�����ŏ������A�X�V�A�`��A�I���������Ǘ����Ă���
// �@�@�@�@ �E�����ɒ��Œl�Ԃ�����Ńe�X�g���Ă邩��A�e�X�g�I������������I
//          
//----------------------------------------------------------------------------------------------------
#include"directx_controller.h"

DirectInputController::DirectInputController()
    : directInput(nullptr), gamepadDevice(nullptr) {}

DirectInputController::~DirectInputController() {
    Release();
}

bool DirectInputController::Initialize(HINSTANCE hInstance, HWND hWnd) {
    // DirectInput�̏�����
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL))) {
        return false;
    }

    // �Q�[���p�b�h�f�o�C�X�̍쐬
    if (FAILED(directInput->CreateDevice(GUID_Joystick, &gamepadDevice, NULL))) {
        return false;
    }

    // �f�[�^�t�H�[�}�b�g��ݒ�
    if (FAILED(gamepadDevice->SetDataFormat(&c_dfDIJoystick))) {
        return false;
    }

    // �������x���̐ݒ�
    if (FAILED(gamepadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) {
        return false;
    }

    // �f�o�C�X�̎擾�J�n
    gamepadDevice->Acquire();

    return true;
}

void DirectInputController::CheckInput() {
    if (gamepadDevice) {
        DIJOYSTATE js;
        HRESULT hr = gamepadDevice->Poll(); // �f�o�C�X�̏�Ԃ��|�[�����O

        if (FAILED(hr)) {
            // �f�o�C�X����A�N�e�B�u�ȏꍇ�Ď擾
            gamepadDevice->Acquire();
        }
        else {
            // ��Ԃ̎擾
            hr = gamepadDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
            if (SUCCEEDED(hr)) {
                // �e�{�^���̏�Ԃ��`�F�b�N
                if (js.rgbButtons[0] & 0x80) { 
                    std::cout << "�{�^��1��������܂����I" << std::endl;
                    //�������ڂ���
                }
                if (js.rgbButtons[1] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //�΂ڂ���
                }
                if (js.rgbButtons[2] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //�܂�ڂ���
                }
                if (js.rgbButtons[3] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //���񂩂��ڂ���
                }





                if (js.rgbButtons[4] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //L1�ڂ���
                }
                if (js.rgbButtons[5] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //R1 �ڂ���
                }
                if (js.rgbButtons[6] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //L2�ڂ���
                }
                if (js.rgbButtons[7] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //R2 �ڂ���
                }



                if (js.rgbButtons[8] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //SHARE�ڂ���
                }
                if (js.rgbButtons[9] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //OPTIONS�ڂ���
                }



                if (js.rgbButtons[10] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //���X�e�B�b�N��������
                }
                if (js.rgbButtons[11] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //�E�X�e�B�b�N������
                }
                if (js.rgbButtons[12] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //�v���C�X�e�[�V�����{�^��
                }


                if (js.rgbButtons[13] & 0x80) {
                    std::cout << "�{�^��2��������܂����I" << std::endl;
                    //�X�N���[���{�^��
                }



                // ���X�e�B�b�N�̌X������
                if (js.lX < -50000) {
                    std::cout << "���X�e�B�b�N�����ɌX���Ă��܂�" << std::endl;
                }
                if (js.lX > 50000) {
                    std::cout << "���X�e�B�b�N���E�ɌX���Ă��܂�" << std::endl;
                }
                if (js.lY < -50000) {
                    std::cout << "���X�e�B�b�N����ɌX���Ă��܂�" << std::endl;
                }
                if (js.lY > 50000) {
                    std::cout << "���X�e�B�b�N�����ɌX���Ă��܂�" << std::endl;
                }


                // �E�X�e�B�b�N�̌X������
                if (js.lRx < -5000) {
                    std::cout << "�E�X�e�B�b�N�����ɌX���Ă��܂�" << std::endl;
                }
                if (js.lRx > 5000) {
                    std::cout << "�E�X�e�B�b�N���E�ɌX���Ă��܂�" << std::endl;
                }
                if (js.lRy < -5000) {
                    std::cout << "�E�X�e�B�b�N����ɌX���Ă��܂�" << std::endl;
                }
                if (js.lRy > 5000) {
                    std::cout << "�E�X�e�B�b�N�����ɌX���Ă��܂�" << std::endl;
                }

                // ���̃{�^�������l�Ƀ`�F�b�N�\
            }
        }
    }
}

void DirectInputController::Release() {
    if (gamepadDevice) {
        gamepadDevice->Unacquire();
        gamepadDevice->Release();
        gamepadDevice = nullptr;
    }
    if (directInput) {
        directInput->Release();
        directInput = nullptr;
    }
}