//-----------------------------------------------------------------------------------------------------
// #name camera_shake
// #description �J�����V�F�C�N
// #make 2025/01/17�@�@�i��`��
// #update 2025/01/17
// #comment �ǉ��E�C���\��
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
