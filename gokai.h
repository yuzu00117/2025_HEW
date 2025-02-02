#ifndef GOKAI_H
#define GOKAI_H

// ---------------------------------------------------------------------------------------------------- -
// #name gokai.h
// #description     gokai��UI�\���p
// #make 2025/02/02�@�i��`��
// #update 2024/02/02
// #comment �ǉ��E�C���\��
//
//----------------------------------------------------------------------------------------------------



#include"main.h"



class Gokai_UI
{

public:
	Gokai_UI() = default;
	~Gokai_UI() {};

	static void	Initialize();
	static void	Finalize();
	static void	Draw();

	// ���݂̃R�C�������擾
	static int GetNowGokaiCount(void)
	{
		return now_get_gokai_count;
	}

	// ���݂̃R�C������ݒ�
	static void SetNowGokaiCount(int setCount)
	{
		now_get_gokai_count = setCount;
	}

	//�ǉ��F���[�����Q�[�W��
	static void AddGokaiCount(int setCount)
	{
		now_get_gokai_count =now_get_gokai_count +setCount ;
	}


private:

	static int max_gokai_count;
	static int now_get_gokai_count;



	
};



#endif // !GOKAI_H
