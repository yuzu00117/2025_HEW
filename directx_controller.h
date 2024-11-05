
#ifndef DIRECTINPUTCONTROLLER_H
#define DIRECTINPUTCONTROLLER_H

#include <windows.h>
#include <dinput.h>
#include <iostream>

class DirectInputController {
public:
    DirectInputController();
    ~DirectInputController();

    bool Initialize(HINSTANCE hInstance, HWND hWnd);
    void CheckInput();
    void Release();

private:
    LPDIRECTINPUT8 directInput;          // DirectInput�C���^�[�t�F�C�X
    LPDIRECTINPUTDEVICE8 gamepadDevice;  // �Q�[���p�b�h�f�o�C�X
};

#endif // DIRECTINPUTCONTROLLER_H