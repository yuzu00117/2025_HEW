/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Timer
 * File     : CriSmpTimer_PC.cpp
 * Version  : 0.94LE
 *
 ****************************************************************************/

#include <stdio.h>
#include <windows.h>
#include <cri_le_xpt.h>
#include "CriSmpTimer.h"

/***
*		Class Functions
***/

class CriSmpTimerLoc : public CriSmpTimer
{
public:
	virtual void Destroy(void);
	virtual void Start(void);
	virtual void Stop(void);
	virtual CriFloat32 GetElapseMsTime(void);
	virtual void PrintElapseMsTime(void);
	virtual void GetTime(CriUint64 &count, CriUint64 &unit);

	LARGE_INTEGER freq;
	LARGE_INTEGER start_cnt;
	LARGE_INTEGER stop_cnt;
	CriBool cnt_flag;
};

CriSmpTimer* CriSmpTimer::Create(void)
{
	CriSmpTimerLoc* tmrl;

	tmrl = new CriSmpTimerLoc;
	QueryPerformanceFrequency(&tmrl->freq);
	QueryPerformanceCounter(&tmrl->start_cnt);
	tmrl->cnt_flag = FALSE;
	tmrl->Start();

	return tmrl;
}

void CriSmpTimerLoc::Destroy(void)
{
	delete this;
}

void CriSmpTimerLoc::Start(void) {
	QueryPerformanceCounter(&this->start_cnt);
	this->cnt_flag = TRUE;
}

void CriSmpTimerLoc::Stop(void) {
	QueryPerformanceCounter(&this->stop_cnt);
	this->cnt_flag = FALSE;
}

CriFloat32 CriSmpTimerLoc::GetElapseMsTime(void)
{
	if ( this->cnt_flag == TRUE ) {
		LARGE_INTEGER ccnt;
		QueryPerformanceCounter(&ccnt);
		return (CriFloat32)(ccnt.QuadPart-this->start_cnt.QuadPart)*1000.0f/this->freq.QuadPart;
	} else {
		return (CriFloat32)(this->stop_cnt.QuadPart-this->start_cnt.QuadPart)*1000.0f
						/(CriFloat32)(this->freq.QuadPart);
	}
}

void CriSmpTimerLoc::PrintElapseMsTime(void)
{
	CriFloat32 etime;

	etime = this->GetElapseMsTime();
	printf("Elapse Time = %f\n", etime);
}

/* second = count / unit */
void CriSmpTimerLoc::GetTime(CriUint64 &count, CriUint64 &unit)
{
	unit = (CriUint64)this->freq.QuadPart;

	if ( this->cnt_flag == TRUE ) {
		LARGE_INTEGER ccnt;
		QueryPerformanceCounter(&ccnt);
		count = (CriUint64)(ccnt.QuadPart-this->start_cnt.QuadPart);
	} else {
		count = (CriUint64)(this->stop_cnt.QuadPart-this->start_cnt.QuadPart);
	}

	return;
}

/* --- end of file --- */