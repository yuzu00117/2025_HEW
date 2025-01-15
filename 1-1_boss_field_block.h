//-----------------------------------------------------------------------------------------------------
// #name boss_field_block.h
// #description �{�X�핔���̃t�B�[���h�𐶐�����H
// #make 2025/01/14�@�i��`��
// #update 2025/01/14
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------

#ifndef BOSS_FIELD_BLOCK_H
#define BOSS_FIELD_BLOCK_H
#include"include/box2d/box2d.h"


enum Boss_Room_Level
{
	null,
	boss_room_level_1,
	boss_room_level_2,
	boss_room_level_3,
};

class boss_field_block
{
public:
	boss_field_block(b2Vec2 position, b2Vec2 size,  int block_hp, Boss_Room_Level level);
	~boss_field_block();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();



	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}


	//�{�f�B���擾
	b2Body* GetBody(void)
	{
		return m_body;
	}

	//�{�f�B���Z�b�g
	void SetBody(b2Body* body)
	{
		m_body = body;
	}



	//�T�C�Y���擾
	b2Vec2 GetSize(void)
	{
		return m_size;
	}
	//�T�C�Y���Z�b�g
	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}

private:

	int id;

	b2Body* m_body;

	b2Vec2 m_size;

	int Block_Hp;

	Boss_Room_Level BossRoomLevel;


};



#endif // BOSS_FIELD_BLOCK_H
