#ifndef	CRISMPVIDEOINPUT_H_INCLUDED		/* Re-definition prevention */
#define	CRISMPVIDEOINPUT_H_INCLUDED
/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Video Output
 * File     : CriSmpVideoOutput.h
 *
 ****************************************************************************/

#include "cri_le_xpt.h"

class CriSmpVideoOutput
{
public:
	static CriSmpVideoOutput* Create(void);
	virtual void Destroy(void)=0;
	virtual void Printf(CriUint32 lx, CriUint32 ly, const char *fmt, ...)=0;
	virtual void WaitVBlank(void)=0;
	virtual void BeginDraw(void)=0;
	virtual void EndDraw(void)=0;

protected:
	CriSmpVideoOutput(void) {}
	virtual ~CriSmpVideoOutput(void) {}
};

void CriSmpPrintf(CriUint32 lx, CriUint32 ly, const char *fmt,...);

#endif		//	CRISMPVIDEOINPUT_H_INCLUDED
