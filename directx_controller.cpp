

//-----------------------------------------------------------------------------------------------------
// #name directx_controler.cpp
// #description game
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
//          ・ここで初期化、更新、描画、終了処理を管理している
// 　　　　 ・ここに直で値ぶち込んでテストしてるから、テスト終わったら消すよ！
//          
//----------------------------------------------------------------------------------------------------
#include"directx_controller.h"

DirectInputController::DirectInputController()
    : directInput(nullptr), gamepadDevice(nullptr) {}

DirectInputController::~DirectInputController() {
    Release();
}

bool DirectInputController::Initialize(HINSTANCE hInstance, HWND hWnd) {
    // DirectInputの初期化
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL))) {
        return false;
    }

    // ゲームパッドデバイスの作成
    if (FAILED(directInput->CreateDevice(GUID_Joystick, &gamepadDevice, NULL))) {
        return false;
    }

    // データフォーマットを設定
    if (FAILED(gamepadDevice->SetDataFormat(&c_dfDIJoystick))) {
        return false;
    }

    // 協調レベルの設定
    if (FAILED(gamepadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) {
        return false;
    }

    // デバイスの取得開始
    gamepadDevice->Acquire();

    return true;
}

void DirectInputController::CheckInput() {
    if (gamepadDevice) {
        DIJOYSTATE js;
        HRESULT hr = gamepadDevice->Poll(); // デバイスの状態をポーリング

        if (FAILED(hr)) {
            // デバイスが非アクティブな場合再取得
            gamepadDevice->Acquire();
        }
        else {
            // 状態の取得
            hr = gamepadDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
            if (SUCCEEDED(hr)) {
                // 各ボタンの状態をチェック
                if (js.rgbButtons[0] & 0x80) { 
                    std::cout << "ボタン1が押されました！" << std::endl;
                    //しかくぼたん
                }
                if (js.rgbButtons[1] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //ばつぼたん
                }
                if (js.rgbButtons[2] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //まるぼたん
                }
                if (js.rgbButtons[3] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //さんかくぼたん
                }





                if (js.rgbButtons[4] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //L1ぼたん
                }
                if (js.rgbButtons[5] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //R1 ぼたん
                }
                if (js.rgbButtons[6] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //L2ぼたん
                }
                if (js.rgbButtons[7] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //R2 ぼたん
                }



                if (js.rgbButtons[8] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //SHAREぼたん
                }
                if (js.rgbButtons[9] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //OPTIONSぼたん
                }



                if (js.rgbButtons[10] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //左スティック押し込み
                }
                if (js.rgbButtons[11] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //右スティック押込み
                }
                if (js.rgbButtons[12] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //プレイステーションボタン
                }


                if (js.rgbButtons[13] & 0x80) {
                    std::cout << "ボタン2が押されました！" << std::endl;
                    //スクリーンボタン
                }



                // 左スティックの傾き判定
                if (js.lX < -50000) {
                    std::cout << "左スティックが左に傾いています" << std::endl;
                }
                if (js.lX > 50000) {
                    std::cout << "左スティックが右に傾いています" << std::endl;
                }
                if (js.lY < -50000) {
                    std::cout << "左スティックが上に傾いています" << std::endl;
                }
                if (js.lY > 50000) {
                    std::cout << "左スティックが下に傾いています" << std::endl;
                }


                // 右スティックの傾き判定
                if (js.lRx < -5000) {
                    std::cout << "右スティックが左に傾いています" << std::endl;
                }
                if (js.lRx > 5000) {
                    std::cout << "右スティックが右に傾いています" << std::endl;
                }
                if (js.lRy < -5000) {
                    std::cout << "右スティックが上に傾いています" << std::endl;
                }
                if (js.lRy > 5000) {
                    std::cout << "右スティックが下に傾いています" << std::endl;
                }

                // 他のボタンも同様にチェック可能
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