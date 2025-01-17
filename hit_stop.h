//-----------------------------------------------------------------------------------------------------
// #name hit_stop.h
// #description ワールドのヒットストップに利用
// #make 2025/01/16　　永野義也
// #update 2025/01/16
// #comment 追加・修正予定
//          ・特になし
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
