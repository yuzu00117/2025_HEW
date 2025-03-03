//-----------------------------------------------------------------------------------------------------
// #name OP.cpp
// #description オープニングシーン
// #make 2025/2/7　王泳心
// #update 2025/2/12
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------
#ifndef OP_H
#define OP_H

#include "Video.h"

class OP
{
public:
	OP();
	~OP();


	static OP& GetInstance() {
		static OP instance;
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


#endif // !OP_H

