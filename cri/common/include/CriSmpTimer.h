#ifndef	CRISMPTIMER_H_INCLUDED		/* Re-definition prevention */
#define	CRISMPTIMER_H_INCLUDED
/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Timer
 * File     : CriSmpTimer.h
 *
 ****************************************************************************/

#include "cri_le_xpt.h"

class CriSmpTimer
{
public:
	static CriSmpTimer* Create(void);
	virtual void Destroy(void)=0;
	virtual void Start(void)=0;
	virtual void Stop(void)=0;
	virtual CriFloat32 GetElapseMsTime(void)=0;
	virtual void PrintElapseMsTime(void)=0;
	virtual void GetTime(CriUint64 &count, CriUint64 &unit)=0; /* sec = count / unit */

protected:
	CriSmpTimer(void) {}
	virtual ~CriSmpTimer(void) {}
};

#endif		//	CRISMPTIMER_H_INCLUDED
