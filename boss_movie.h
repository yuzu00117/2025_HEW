//-----------------------------------------------------------------------------------------------------
// #name boss_movie.h
// #description ボスの登場ムービーを再生するhファイル
// #make 2025/03/04　yuzu00117
// #update 2025/03/04
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------
#ifndef BOSS_MOVIE_H
#define BOSS_MOVIE_H

#include "Video.h"

class BossMovie
{
public:
	BossMovie();
	~BossMovie();


	static BossMovie& GetInstance() {
		static BossMovie instance;
		return instance;
	}






	void Initialize();
	void Update();
	void Draw();
	void Finalize();


private:

	Video video;
	bool m_pause;

	float skip_cnt;


};


#endif // !BossMovie_H

