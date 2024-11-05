//-----------------------------------------------------------------------------------------------------
// #name texture.h
// #description テクスチャ！
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 　授業通りでしょ！
//          
//----------------------------------------------------------------------------------------------------

#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "main.h"
#include "renderer.h"

ID3D11ShaderResourceView* InitTexture(const wchar_t* textureName);
void UnInitTexture(ID3D11ShaderResourceView*);

#endif//_TEXTURE_H
