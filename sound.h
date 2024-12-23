
//-----------------------------------------------------------------------------------------------------
// #name sound.h
// #description ���I
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
// 
// �@�@�@�@�@�@�E�������ƂȂ���@���ƒʂ�
// �@�@�@�@�@�@�ECRI�ł���Ƃ����ȁ[
//   
//          
//----------------------------------------------------------------------------------------------------

#ifndef SOUND_H

#define SOUND_H

#include <windows.h>
#include "xaudio2.h"
#include "cri_adx2le.h"


bool InitSound(HWND hWnd);
void UninitSound(void);

int LoadSound(char* pFilename);
void PlaySound(int index, int loopCount);
void StopSound(int index);
void StopSoundAll(void);
void SetVolume(int index, float vol);

#endif // !SOUND_H