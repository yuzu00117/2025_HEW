/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Input Device
 * File     : CriSmpInputDevice_PC.cpp
 * Version  : 0.92LE
 *
 ****************************************************************************/

#include <windows.h>
#include <cri_le_xpt.h>
#include "CriSmpInputDevice.h"

/* Warning prevention */
#define UNUSED(arg)						{ if ((arg) == (arg)) {} }

/***************************************************************************
 *      変数定義
 *      Variable Definitions
 ***************************************************************************/
static int g_key_map[] =
{
	'1',
	'2',
	'3',
	'4',
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,
	VK_ESCAPE,
	VK_RETURN,
};

/* ウィンドウハンドルの取得 */
extern HWND CriSmpFramework_GetWindowHandle(void);

/***************************************************************************
 *      クラス宣言
 *      Class Declarations
 ***************************************************************************/
class CriSmpInputDeviceLoc : public CriSmpInputDevice
{
public:
	static CriSmpInputDeviceLoc* Create(void);
	/* 純粋仮想関数の実装 */
	void Destroy(void);
	CriBool IsPressed(CriUint32 button);
	CriBool IsOn(CriUint32 button);
	void Update(void);
private:
	CriSmpInputDeviceLoc(void) {}
	~CriSmpInputDeviceLoc(void) {}

private:
	CriBool is_pressed[NUM_BUTTONS];
	CriBool is_on[NUM_BUTTONS];
};


/****
*			CriSmpInputDevice
***/
CriSmpInputDevice* CriSmpInputDevice::Create(void)
{
	return CriSmpInputDeviceLoc::Create();
}

/***
*		Class Functions
***/

CriSmpInputDeviceLoc* CriSmpInputDeviceLoc::Create(void)
{
	CriSmpInputDeviceLoc* idev = new CriSmpInputDeviceLoc;

	for (int i=0;i<NUM_BUTTONS;i++) {
		idev->is_pressed[i] = CRI_FALSE;
		idev->is_on[i] = CRI_FALSE;
	}

	return idev;
}

CriBool CriSmpInputDeviceLoc::IsPressed(CriUint32 button)
{
	return this->is_pressed[button];
}

CriBool CriSmpInputDeviceLoc::IsOn(CriUint32 button)
{
	return this->is_on[button];
}

void CriSmpInputDeviceLoc::Destroy(void)
{
	delete this;
}

void CriSmpInputDeviceLoc::Update(void)
{
	SHORT key_state;

	/* ウィンドウ判定 */
	HWND hwnd;
	hwnd = CriSmpFramework_GetWindowHandle();
	if (hwnd != GetActiveWindow()) {
		return;
	}

	/* キー入力情報の更新 */
	for (int i=0;i<NUM_BUTTONS;i++) {

		/* キー状態の取得 */
		key_state = GetAsyncKeyState(g_key_map[i]);

		/* キーが押されているか */
		if (key_state & 0x8000) {
			/* 押されたかどうかは前回の状態で判定 */
			if (is_pressed[i] == CRI_FALSE && is_on[i] == CRI_FALSE) {
				is_pressed[i] = CRI_TRUE;
			} else {
				is_pressed[i] = CRI_FALSE;
			}
			is_on[i] = CRI_TRUE;
		} else {
			/* 状態クリア */
			is_pressed[i] = CRI_FALSE;
			is_on[i] = CRI_FALSE;
		}
	}
	return;
}

/* eof */
