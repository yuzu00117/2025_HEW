#ifndef ANCHOR_POINT
#define ANCHOR_POINT
#include"field.h"
#include<vector>


class AnchorPoint:public Field
{
public:
	AnchorPoint(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~AnchorPoint();



	/**
	 * @brief  �Z���T�[�ɓ�������Vector�ɃA���J�[�|�C���g��Body��n���ăv�b�V������
	 * @param body �Z���T�[�ɓ������A���J�[�|�C���g�� body
	 */
	static void InsideSensor(b2Body* new_anchor_point_body);


	/**
	 * @brief  �Z���T�[����ł��A���J�[�|�C���g��Body��Vector�Ȃ��̃A���J�[�|�C���g�̃{�f�B���r����
	 �@�Ή����Ă�����̂�Vector�������
	 * @param body �Z���T�[����ł��A���J�[�|�C���g�� body
	 */
	static void OutsideSensor(b2Body* delete_anchor_point_body);

	/**
	 * @brief �Z���T�[���̃A���J�[�|�C���g�̒�����@
			  �X�e�B�b�N�̑���ŃA���J�[�|�C���g��I������
	 * @param  
	 */
	static void SelectAnchorPoint(float stick_x,float stick_y);


	static void Initialize();
	static void Update();
	static void Draw();

	/**
 * @brief �@�^�[�Q�b�g�Ƃ����A���J�[�|�C���g�̃{�f�B���擾����֐�
 * @return  �^�[�Q�b�g�Ƃ����A���J�[�|�C���g�̃{�f�B
 */
	static b2Body* GetTargetAnchorPointBody();

private:
	
};



#endif // !ANCHOR_POINT

