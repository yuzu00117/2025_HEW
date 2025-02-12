#pragma once

#include"video_texture.h"
#include<string>

enum Video_State
{
	Video_Start,
	Video_Pause,
	Video_Resume,
	Video_Skip,
	Video_Finished,	
};

class Video
{
public:
	Video();
	~Video();


	void Initialize(const std::string& fileName);
	//動画終わったかを返す
	bool Update();
	void Draw(const XMFLOAT2& pos = XMFLOAT2(0.0f, 0.0f), float height = 1000);
	void Finalize();


	Video_State	GetState() { return m_state; }
	void	SetState(Video_State state) { 

		if (state == Video_Finished) {
			return;
		}

		m_state = state; 

		switch (m_state)
		{
		case Video_Start:
			break;
		case Video_Pause:
			m_video.pause();
			break;
		case Video_Resume:
			m_video.resume();
			break;
		case Video_Skip:
			Finalize();
			break;
		}
	}

	bool	GetIfFinished() { return m_video.hasFinished(); }
	

private:
	//動画の状態
	Video_State m_state;
	//動画
	VideoTexture m_video;
};

