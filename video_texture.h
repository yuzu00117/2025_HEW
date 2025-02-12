//-----------------------------------------------------------------------------------------------------
// #name video_texture.h
// #description ����̓�������
// #make �@�@Juan Abadia�i�ȉ���URL�������j
// https://github.com/yabadabu/dx11_video_texture
// #update 
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------

#ifndef VIDEO_TEXTURE_H
#define VIDEO_TEXTURE_H

#include "renderer.h"
#include "MovieTexture.h"

#define SAFE_RELEASE(x) if(x) { (x)->Release(); x = nullptr; }


struct VideoTexture {

	static bool createAPI();
	static void destroyAPI();

	struct InternalData;
	InternalData* internal_data = nullptr;

	bool create(const char* filename);
	void destroy();
	bool update(float dt);

	void pause();
	void resume();
	bool hasFinished();
	MovieTexture* getTexture();
	float getAspectRatio() const;
};

#endif // !VIDEO_TEXTURE_H


