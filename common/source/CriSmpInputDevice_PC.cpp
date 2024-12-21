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
 *      �ϐ���`
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

/* �E�B���h�E�n���h���̎擾 */
extern HWND CriSmpFramework_GetWindowHandle(void);

/***************************************************************************
 *      �N���X�錾
 *      Class Declarations
 ***************************************************************************/
class CriSmpInputDeviceLoc : public CriSmpInputDevice
{
public:
	static CriSmpInputDeviceLoc* Create(void);
	/* �������z�֐��̎��� */
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

	/* �E�B���h�E���� */
	HWND hwnd;
	hwnd = CriSmpFramework_GetWindowHandle();
	if (hwnd != GetActiveWindow()) {
		return;
	}

	/* �L�[���͏��̍X�V */
	for (int i=0;i<NUM_BUTTONS;i++) {

		/* �L�[��Ԃ̎擾 */
		key_state = GetAsyncKeyState(g_key_map[i]);

		/* �L�[��������Ă��邩 */
		if (key_state & 0x8000) {
			/* �����ꂽ���ǂ����͑O��̏�ԂŔ��� */
			if (is_pressed[i] == CRI_FALSE && is_on[i] == CRI_FALSE) {
				is_pressed[i] = CRI_TRUE;
			} else {
				is_pressed[i] = CRI_FALSE;
			}
			is_on[i] = CRI_TRUE;
		} else {
			/* ��ԃN���A */
			is_pressed[i] = CRI_FALSE;
			is_on[i] = CRI_FALSE;
		}
	}
	return;
}

/* eof */
