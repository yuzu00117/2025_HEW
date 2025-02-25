//-----------------------------------------------------------------------------------------------------
// #name texture.cpp
// #description テクスチャ
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
//
//           授業通り！　いうことないわ
//          
//----------------------------------------------------------------------------------------------------

#include "texture.h"

ID3D11ShaderResourceView* InitTexture(const wchar_t* textureName) {
	ID3D11ShaderResourceView* Texture;
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(textureName, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &Texture);
	assert(Texture);

	return Texture;
}

void UnInitTexture(ID3D11ShaderResourceView*&texture) {
	if (!texture) 
	{	
		return;
	}

	if (texture != nullptr)
	{
		texture->Release();
		texture = nullptr;
	}
}