//-----------------------------------------------------------------------------------------------------
// #name KeyInput_Flag.h
// #description     �L�[���͗����i�O�t���[���L�[�������ꂽ���ǂ����Ɏg���j
// #make 2025/2/28�@���j�S
// #update 2025/2/28
// #comment �ǉ��E�C���\��
//                  
//                  
//----------------------------------------------------------------------------------------------------
#ifndef KEYINPUT_FLAG_H
#define	KEYINPUT_FLAG_H

class Button_PressFlag
{
public:
	float  CountTime = 0.0f;		//���ԑ��邽�߂̕ϐ��A���Ɏg���Ă�������
	bool ControllerButton_A = false;
	bool ControllerButton_B = false;
	bool ControllerButton_X = false;
	bool ControllerButton_Y = false;
	bool ControllerButton_Up = false;
	bool ControllerButton_Down = false;
	bool ControllerButton_Left = false;
	bool ControllerButton_Right = false;
	bool ControllerButton_L = false;
	bool ControllerButton_ZL = false;
	bool ControllerButton_R = false;
	bool ControllerButton_ZR = false;
	bool ControllerButton_Start = false;

	bool KeyboardButton_Escape = false;
	bool KeyboardButton_TAB = false;
	bool KeyboardButton_Enter = false;
	bool KeyboardButton_Up = false;
	bool KeyboardButton_Down = false;
	bool KeyboardButton_Left = false;
	bool KeyboardButton_Right = false;
	bool KeyboardButton_Shift_Left = false;
	bool KeyboardButton_Shift_Right = false;
	bool KeyboardButton_CTRL_Left = false;
	bool KeyboardButton_CTRL_Right = false;
	bool KeyboardButton_Space = false;
	bool KeyboardButton_A = false;
	bool KeyboardButton_B = false;
	bool KeyboardButton_D = false;
	bool KeyboardButton_E = false;
	bool KeyboardButton_F = false;
	bool KeyboardButton_G = false;
	bool KeyboardButton_H = false;
	bool KeyboardButton_I = false;
	bool KeyboardButton_J = false;
	bool KeyboardButton_L = false;
	bool KeyboardButton_M = false;
	bool KeyboardButton_N = false;
	bool KeyboardButton_O = false;
	bool KeyboardButton_Q = false;
	bool KeyboardButton_R = false;
	bool KeyboardButton_S = false;
	bool KeyboardButton_T = false;
	bool KeyboardButton_U = false;
	bool KeyboardButton_V = false;
	bool KeyboardButton_W = false;
	bool KeyboardButton_X = false;
	bool KeyboardButton_Y = false;
	bool KeyboardButton_Z = false;
};

#endif // !KEYINPUT_FLAG_H
