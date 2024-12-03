#ifndef	CRISMPINPUTDEVICE_H_INCLUDED		/* Re-definition prevention */
#define	CRISMPINPUTDEVICE_H_INCLUDED
/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Input Device
 * File     : CriSmpInputDevice.h
 *
 ****************************************************************************/

#include "cri_le_xpt.h"

class CriSmpInputDevice
{
public:
	enum Button {
		BUTTON1,
		BUTTON2,
		BUTTON3,
		BUTTON4,
		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_CANCEL,
		BUTTON_OK,

		NUM_BUTTONS
	};
	static CriSmpInputDevice* Create(void);
	virtual void Destroy(void)=0;
	virtual CriBool IsPressed(CriUint32 button)=0;
	virtual CriBool IsOn(CriUint32 button)=0;
	virtual void Update(void)=0;

protected:
	CriSmpInputDevice(void) {}
	virtual ~CriSmpInputDevice(void) {}
};

#endif		//	CRISMPINPUTDEVICE_H_INCLUDED
