//-----------------------------------------------------------------------------------------------------
// #name texture.h
// #description �e�N�X�`���I
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �@���ƒʂ�ł���I
//          
//----------------------------------------------------------------------------------------------------

#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "main.h"
#include "renderer.h"

ID3D11ShaderResourceView* InitTexture(const wchar_t* textureName);
void UnInitTexture(ID3D11ShaderResourceView*);

#endif//_TEXTURE_H
