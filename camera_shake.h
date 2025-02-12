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

#include"include/box2d/box2d.h"

class CameraShake
{
public:

	static void Update(void);


	static void StartCameraShake(int shake_height,int shake_width,int shake_frame)
	{

		//�J�����V�F�C�N�̃X�^�[�g���Ăяo���ƁA�����ɂ��̃J�����V�F�C�N�Ɉڍs���Ă��܂����߁@
		//�Ⴆ�΃{�X�̑傫�Ȑk�x���v���C���[���G��|���������Ȑk�x�ŏ㏑���ł��Ă��܂���肪�������Ă����̂ł�����C������
		if (shake_height + shake_width < camera_shake_height + camera_shake_width)
		{
			return;
		}


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

	static b2Vec2 old_olayer_pos;

};



#endif // !CAMERA_SHAKE_H
