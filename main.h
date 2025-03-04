
//-----------------------------------------------------------------------------------------------------
// #name main.h
// #description main 
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
//          �E��{�I�Ɏ��ƒʂ�̃e���v���[�g���������Ă���
//          
//----------------------------------------------------------------------------------------------------
#ifndef _MAIN_H
#define _MAIN_H

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#include <stdio.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
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
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���
#define SCREEN_XCENTER  (640)
#define SCREEN_YCENTER  (360)

#define PIECE_WIDTH     (50)                //�s�[�X�T�C�Y
#define PIECE_HEIGHT    (50)                //�s�[�X�T�C�Y

#define BLOCK_COLS      (6)
#define BLOCK_ROWS      (13)

#define EFFECT_MAX      (100)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT FirstInit(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

void FinalFinalize();

#endif //_MAIN_H