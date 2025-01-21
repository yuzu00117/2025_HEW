//-----------------------------------------------------------------------------------------------------
// #name camera_shake
// #description カメラシェイク
// #make 2025/01/17　　永野義也
// #update 2025/01/17
// #comment 追加・修正予定
//          
// 
//----------------------------------------------------------------------------------------------------

#ifndef CAMERA_SHAKE_H
#define CAMERA_SHAKE_H
class CameraShake
{
public:

	static void Update(void);


	static void StartCameraShake(int shake_height,int shake_width,int shake_frame)
	{
		camera_shake_flag = true;
		camera_shake_height = shake_height;
		camera_shake_width=shake_width;
		camera_shake_frame = shake_frame;
	}




	

private:

	static bool camera_shake_minus;

	static bool camera_shake_flag;

	static int camera_shake_frame;

	static int camera_shake_height;

	static int camera_shake_width;

};



#endif // !CAMERA_SHAKE_H
