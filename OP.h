//-----------------------------------------------------------------------------------------------------
// #name OP.cpp
// #description �I�[�v�j���O�V�[��
// #make 2025/2/7�@���j�S
// #update 2025/2/12
// #comment �ǉ��E�C���\��
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

