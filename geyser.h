//-----------------------------------------------------------------------------------------------------
// #name geyser.h
// #description �Ԍ���̃w�b�_�[
// #make 2024/12/27�@�@�i��`��
// #update 2024/12/27
// #comment �ǉ��E�C���\��
//                      �E���ɂȂ�
//           
//----------------------------------------------------------------------------------------------------

#ifndef GEYSER_H
#define GEYSER_H

#include"include/box2d/box2d.h"
#include<vector>
#include"1-1_boss.h"
#include"1-1_boss_field_block.h"

class geyser
{
public:

	geyser(b2Vec2 GeyserPosition, b2Vec2 GeyserSize, b2Vec2 RangeFlyWaterSize, int splitting_x, int splitting_y, int level);
	~geyser();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Destroy_Joint();

	void PullingOnRock();

	void JumpPlayer();

	
	void Destroy_Splitting();

	//�Ԃ񂩂�����
	void DestroySplittedBodies(std::vector<b2Body*>& bodyList);

	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}



	//�Ԍ���body�̏K��
	b2Body* GetGeyserBody()
	{
		return geyser_body;
	}
	void SetGeyserBody(b2Body* body)
	{
		geyser_body = body;
	}


	//�Ԍ���ɃT�C�Y���������Ă���
	b2Vec2 GetGeyserSize() const
	{
		return geyser_size;
	}

	void SetGeyserSize(b2Vec2 size) {
		geyser_size = size;
	}




	b2Body* GetAnchorPointBody()
	{
		return Anchor_point_body;
	}

	void SetAnchorPointBody(b2Body* body)
	{
		Anchor_point_body = body;
	}



	//�Ԍ���̐��̔�Ԕ͈͂̐ݒ�
	b2Vec2 GetRangeFlyWaterSize() const
	{
		return range_fly_water_size;
	}

	void SetRangeFlyWaterSize(b2Vec2 size) {
		range_fly_water_size = size;
	}





	//�Ԍ���̏�ɏ���Ă��̃{�f�B
	b2Body* GetGeyserOnRockBody()
	{
		return geyser_on_rock_body;
	}

	void SetGeyserOnRockBody(b2Body* body)
	{
		geyser_on_rock_body = body;
	}


	// �Ԍ���̏�ɏ���Ă��̃T�C�Y
	b2Vec2 GetGeyserOnRockSize() const
	{
		return geyser_on_rock_size;
	}

	void SetGeyserOnRockSize(b2Vec2 size)
	{
		geyser_on_rock_size = size;
	}


	bool GetFlag()const
	{
		return water_flag;
	}

	void SetFlag(bool flag)
	{
		water_flag = flag;
	}


	void SetOpenGyserFlag(bool flag)
	{
		open_gyeser_flag = flag;
	}



private:

	bool isUse;

	int id; // �e�C���X�^���X�ŗL�� ID

	b2Body* geyser_body;//�Ԍ���̃{�f�B

	b2Vec2 geyser_size;//�Ԍ���̃T�C�Y



	b2Body* Anchor_point_body;




	b2Vec2 range_fly_water_size;//�Ԍ���̐�����Ԕ͈�


	b2Body* geyser_on_rock_body;//�Ԍ���ɏ���Ă��̃{�f�B

	b2Vec2 geyser_on_rock_size;//�Ԍ���ɏ���Ă��̃T�C�Y


	bool water_flag;//�����v���C���[�Ƃӂ�Ă���

	float easing_rate;


	int Splitting_x;//���̕�������

	int Splitting_y;//���Ă̕�������

	bool Splitting_Destroy_Flag = false;//���̃{�f�B��j�󂵂ĕ�������t���O

	bool Splitting_end = false;//�����I��

	int Destroy_Cnt = 0;

	bool open_gyeser_flag = false;

	//�X�v���C�g�̊Ǘ��p
	float draw_cnt;

	int water_sheet_cnt = 0;

	Boss_Room_Level boss_room_level;
	//�����o���o���ɂȂ����Ƃ��Ɏg��
	std::vector<b2Body*> boss_geyser_body_Splitting;



};


#endif //