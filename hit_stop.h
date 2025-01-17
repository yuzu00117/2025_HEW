//-----------------------------------------------------------------------------------------------------
// #name hit_stop.h
// #description ���[���h�̃q�b�g�X�g�b�v�ɗ��p
// #make 2025/01/16�@�@�i��`��
// #update 2025/01/16
// #comment �ǉ��E�C���\��
//          �E���ɂȂ�
//----------------------------------------------------------------------------------------------------


#ifndef HIT_STOP_H
#define HIT_STOP_H

class HitStop
{
public:
	static bool GetHitStopFlag(void)
	{
		return hit_stop_flag;
	}

	static void SetHitStopFlag(bool flag)
	{
		hit_stop_flag = flag;
	}

	static void StartHitStop(int hit_stop_frame)
	{
		SetHitStopFlag(true);

		hit_stop_time = hit_stop_frame;
	}


	static void CountHitStop(void)
	{
		hit_stop_time--;
		if (hit_stop_time <= 0)
		{
			SetHitStopFlag(false);
			hit_stop_time = 0;

		}
	}

private:

	static bool hit_stop_flag;

	static int hit_stop_time;
};



#endif // !HIT_STOP_H
