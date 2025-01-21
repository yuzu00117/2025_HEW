//-----------------------------------------------------------------------------------------------------
// #name camera_shake
// #description �J�����V�F�C�N
// #make 2025/01/17�@�@�i��`��
// #update 2025/01/17
// #comment �ǉ��E�C���\��
//          
// 
//----------------------------------------------------------------------------------------------------

#include"display.h"
#include"camera_shake.h"
#include"tool.h"

bool CameraShake::camera_shake_minus = false;
bool CameraShake::camera_shake_flag = false;
int CameraShake::camera_shake_frame = 0;
int CameraShake::camera_shake_height = 0;
int CameraShake::camera_shake_width = 0;


void CameraShake::Update()
{
	if (camera_shake_flag == true)
	{
		camera_shake_frame--;

		float random = GetRandomInt(5, 10);
		random = random * 0.1;
		
		if (camera_shake_frame % 4 == 0)
		{
			if (camera_shake_minus)
			{
				display::SetDisplayHeight(random * camera_shake_height * -1);
				display::SetDisplayWidth(random * camera_shake_width * -1);
				camera_shake_minus = false;
			}
			else
			{
				display::SetDisplayHeight(random * camera_shake_height);
				display::SetDisplayWidth(random * camera_shake_width);
				camera_shake_minus = true;
			}
		}
	

		//�J�����V�F�C�N�I��
		if (camera_shake_frame <= 0)
		{
			//���Z�b�g����
			camera_shake_flag = false;
			camera_shake_height = 0;
			camera_shake_width = 0;

			display::SetDisplayDefault();

		}
	}
}