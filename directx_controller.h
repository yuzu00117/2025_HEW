
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
    LPDIRECTINPUT8 directInput;          // DirectInputインターフェイス
    LPDIRECTINPUTDEVICE8 gamepadDevice;  // ゲームパッドデバイス
};

#endif // DIRECTINPUTCONTROLLER_H