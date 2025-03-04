
//-----------------------------------------------------------------------------------------------------
// #name main.h
// #description main 
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
//          ・基本的に授業通りのテンプレート引っ張ってきた
//          
//----------------------------------------------------------------------------------------------------
#ifndef _MAIN_H
#define _MAIN_H

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"




#pragma warning(pop)



#include <DirectXMath.h>
using namespace DirectX;

#include "DirectXTex.h"

#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

#ifdef _DEBUG
#define new DBG_NEW
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ
#define SCREEN_XCENTER  (640)
#define SCREEN_YCENTER  (360)

#define PIECE_WIDTH     (50)                //ピースサイズ
#define PIECE_HEIGHT    (50)                //ピースサイズ

#define BLOCK_COLS      (6)
#define BLOCK_ROWS      (13)

#define EFFECT_MAX      (100)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT FirstInit(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

void FinalFinalize();

#endif //_MAIN_H