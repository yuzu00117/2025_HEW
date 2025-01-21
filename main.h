
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


// オブジェクトの種類を定義
enum ObjectType {
    NULL_object,
    Object_Wood, // 木
    Object_Rock, // 岩
    Object_one_way_platform,//足場　したからしか乗れない
    Object_Static_to_Dynamic,//静的から動的に変更するオブジェクト
    Object_Movable_Ground,  //引っ張れる床 

    Object_Enemy_Static,//静的エネミー
    Object_Enemy_Dynamic,//動的エネミー
    Object_Enemy_Attack, //エネミーの攻撃
    Object_Enemy_Floating,//浮遊エネミー

    Object_teleport_block,//テレポートブロック
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT FirstInit(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

void FinalFinalize();

#endif //_MAIN_H