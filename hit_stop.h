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


	//�q�b�g�X�g�b�v���n�߂�
	static void StartHitStop(int hit_stop_frame)
	{
		SetHitStopFlag(true);

		hit_stop_time = hit_stop_frame;
	}

	//�t���[���҂��ăq�b�g�X�g�b�v���n�߂�
	static void DelayStartHitStop(int hit_stop_frame, int delaytime)
	{
		delay_hit_stop_time = hit_stop_frame;
		delay_time = delaytime;
	}

	static void DelayCountHitStop(void)
	{
		if (delay_time != 0)
		{
			delay_time--;

			if (delay_time == 0)
			{
				StartHitStop(delay_hit_stop_time);
				delay_hit_stop_time = 0;
			}
		}
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

	static int delay_time;//�҂����Ԃ�

	static int delay_hit_stop_time;


};



#endif // !HIT_STOP_H
