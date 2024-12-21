#ifndef	CRISMPFRAMEWORK_H_INCLUDED		/* Re-definition prevention */
#define	CRISMPFRAMEWORK_H_INCLUDED
/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Framework
 * File     : CriSmpFramework.h
 * 
 ****************************************************************************/

#include "cri_le_xpt.h"
#include "CriSmpInputDevice.h"
#include "CriSmpVideoOutput.h"
#include "CriSmpTimer.h"

class CriSmpFramework
{
public:
	static CriSmpFramework* Create(void);
	virtual void Destroy(void)=0;
	virtual CriBool SyncFrame(void)=0;
	virtual void BeginDraw(void)=0;
	virtual void EndDraw(void)=0;
	virtual CriBool IsTerninated(void)=0;

	CriSmpInputDevice* GetInputDevice(void) {
		return this->idev;
	}
	CriSmpVideoOutput* GetVideoOutput(void) {
		return this->vout;
	}
	CriSmpTimer* GetTimer(void) {
		return this->timer;
	}

protected:
	CriSmpFramework(void) {}
	virtual ~CriSmpFramework(void) {}

	CriSmpInputDevice* idev;
	CriSmpVideoOutput* vout;
	CriSmpTimer* timer;
};

void CriSmpDebugPrintf(const char *fmt, ...);

#endif		//	CRISMPFRAMEWORK_H_INCLUDED
