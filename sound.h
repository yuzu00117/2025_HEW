
//-----------------------------------------------------------------------------------------------------
// #name sound.h
// #description 音！
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
// 
// 　　　　　　・いうことないわ　授業通り
// 　　　　　　・CRIできるといいなー
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