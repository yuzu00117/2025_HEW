//-----------------------------------------------------------------------------------------------------
// #name texture.cpp
// #description �e�N�X�`��
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
//
//           ���ƒʂ�I�@�������ƂȂ���
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