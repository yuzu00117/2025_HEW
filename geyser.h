#ifndef GEYSER_H
#define GEYSER_H

#include"include/box2d/box2d.h"

class geyser
{
public:

	geyser(b2Vec2 GeyserPosition, b2Vec2 GeyserSize, b2Vec2 RangeFlyWaterSize, b2Vec2 GeyserOnRockSize, int set_rock_need_anchor_level);
	~geyser();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Destroy_Joint();

	void PullingOnRock();

	void JumpPlayer();


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


	bool Get_geyser_on_rock_flag()const
	{
		return geyser_on_rock_flag;
	}

	void Set_geyser_on_rock_flag(bool flag)
	{
		geyser_on_rock_flag = flag;
	}

private:
	int id; // �e�C���X�^���X�ŗL�� ID

	b2Body* geyser_body;//�Ԍ���̃{�f�B

	b2Vec2 geyser_size;//�Ԍ���̃T�C�Y

	b2Vec2 range_fly_water_size;//�Ԍ���̐�����Ԕ͈�


	b2Body* geyser_on_rock_body;//�Ԍ���ɏ���Ă��̃{�f�B

	b2Vec2 geyser_on_rock_size;//�Ԍ���ɏ���Ă��̃T�C�Y


	bool geyser_on_rock_flag;//��̏�ɊԌ��򂪏���Ă�




};


#endif //