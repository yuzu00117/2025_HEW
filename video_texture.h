//-----------------------------------------------------------------------------------------------------
// #name video_texture.h
// #description 動画の内部処理
// #make 　　Juan Abadia（以下のURLが索引）
// https://github.com/yabadabu/dx11_video_texture
// #update 
// #comment 追加・修正予定
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


