/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006 CRI Middleware Co., Ltd.
 *
 * Library  : Sample Library
 * Module   : Video Output
 * File     : CriSmpVideoOutput_PC.cpp
 * Version  : 1.02LE
 *
 ****************************************************************************/

/***************************************************************************
 *      インクルードファイル
 *      Include files
 ***************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <cri_le_xpt.h>
#include "CriSmpVideoOutput.h"

/***************************************************************************
 *      定数マクロ
 *      Macro Constants
 ***************************************************************************/

/***************************************************************************
 *      処理マクロ
 *      Macro Functions
 ***************************************************************************/

/***************************************************************************
 *      データ型宣言
 *      Data Type Declarations
 ***************************************************************************/
class CriSmpVideoOutputLoc : public CriSmpVideoOutput
{
public:
	static CriSmpVideoOutputLoc* Create();
	virtual void Destroy(void);
	virtual void Printf(CriUint32 lx, CriUint32 ly, const char *fmt, ...);
	virtual void WaitVBlank(void) {}
	virtual void BeginDraw(void);
	virtual void EndDraw(void);

	void Vprintf(CriUint32 lx, CriUint32 ly, const char *fmt, va_list args);
	static CriSmpVideoOutputLoc* GetInstance();

private:
	static const CriUint32 FONT_WIDTH = 9;
	static const CriUint32 FONT_HEIGHT = 18;
	CriSmpVideoOutputLoc();
	~CriSmpVideoOutputLoc();

	HWND	hwnd;
	HDC		hdc;
	HDC		hmemdc;
	HBITMAP	hbitmap;
	HBITMAP	holdbitmap;
	HFONT	hfont;
	HFONT	holdfont;
	RECT	rect;
	CriSint32	width;
	CriSint32	height;

	static CriSint32 create_count;
	static CriSmpVideoOutputLoc* voutl_instance;
};

/***************************************************************************
 *      変数宣言
 *      Prototype Variables
 ***************************************************************************/

/***************************************************************************
 *      関数宣言
 *      Prototype Functions
 ***************************************************************************/
extern HWND CriSmpFramework_GetWindowHandle(void);

/***************************************************************************
 *      変数定義
 *      Variable Definition
 ***************************************************************************/
CriSint32 CriSmpVideoOutputLoc::create_count = 0;
CriSmpVideoOutputLoc* CriSmpVideoOutputLoc::voutl_instance = NULL;

/***************************************************************************
 *      関数定義
 *      Function Definition
 ***************************************************************************/
/***
*		Class Functions
***/
CriSmpVideoOutput* CriSmpVideoOutput::Create(void)
{
	return CriSmpVideoOutputLoc::Create();
}

CriSmpVideoOutputLoc* CriSmpVideoOutputLoc::Create(void)
{
	CriSmpVideoOutputLoc* voutl;

	if (CriSmpVideoOutputLoc::create_count == 0) {
		voutl = new CriSmpVideoOutputLoc;
		CriSmpVideoOutputLoc::voutl_instance = voutl;
	} else {
		voutl = CriSmpVideoOutputLoc::voutl_instance;
	}

	CriSmpVideoOutputLoc::create_count++;

	return voutl;
}


void CriSmpVideoOutputLoc::Destroy(void)
{
	CriSmpVideoOutputLoc::create_count--;

	if (CriSmpVideoOutputLoc::create_count == 0) {
		delete this;
		CriSmpVideoOutputLoc::voutl_instance = NULL;
	}
}

CriSmpVideoOutputLoc::CriSmpVideoOutputLoc()
{
	this->hwnd = CriSmpFramework_GetWindowHandle();
	
	GetClientRect(this->hwnd, &this->rect);
	this->width = this->rect.right - this->rect.left;
	this->height = this->rect.bottom - this->rect.top;

	this->hdc = GetDC(this->hwnd);
	this->hmemdc = CreateCompatibleDC(this->hdc);
	this->hbitmap = CreateCompatibleBitmap(this->hmemdc, this->width, this->height);
	this->holdbitmap = (HBITMAP)SelectObject(this->hmemdc, this->hbitmap);

	this->hfont = CreateFont(FONT_HEIGHT, FONT_WIDTH, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, "Terminal");
	this->holdfont = (HFONT)SelectObject(this->hmemdc, this->hfont);

	SetTextColor(this->hmemdc, RGB(0, 0, 0));
	SetBkColor(this->hmemdc, RGB(255, 255, 255));
}

CriSmpVideoOutputLoc::~CriSmpVideoOutputLoc()
{
	SelectObject(this->hmemdc, this->holdfont);
	DeleteObject(this->hfont);
	SelectObject(this->hmemdc, this->holdbitmap);
	DeleteDC(this->hmemdc);
	ReleaseDC(this->hwnd, this->hdc);
}

void CriSmpVideoOutputLoc::Printf(CriUint32 lx, CriUint32 ly, const char *fmt,...)
{
	va_list args;

	va_start(args, fmt);
	Vprintf(lx, ly, fmt, args);
	va_end(args);
}

void CriSmpVideoOutputLoc::BeginDraw()
{
	FillRect(this->hmemdc, &this->rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

void CriSmpVideoOutputLoc::EndDraw()
{
	BitBlt(this->hdc, 0, 0, this->width, this->height, this->hmemdc, 0, 0, SRCCOPY);
}

void CriSmpVideoOutputLoc::Vprintf(CriUint32 lx, CriUint32 ly, const char *fmt, va_list args)
{
	CriChar8 string[512];
	
	vsprintf_s(string, sizeof(string), fmt, args);

	TextOut(this->hmemdc, lx*FONT_WIDTH, ly*FONT_HEIGHT, string, (int)strlen(string)); 
}

CriSmpVideoOutputLoc* CriSmpVideoOutputLoc::GetInstance()
{
	return CriSmpVideoOutputLoc::voutl_instance;
}

void CriSmpPrintf(CriUint32 lx, CriUint32 ly, const char *fmt,...)
{
	CriSmpVideoOutputLoc* voutl = CriSmpVideoOutputLoc::GetInstance();
	if (voutl == NULL) {
		return;
	}

	va_list args;

	va_start(args, fmt);
	voutl->Vprintf(lx, ly, fmt, args);
	va_end(args);
}

/* --- end of file --- */
