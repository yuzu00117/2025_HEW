//-----------------------------------------------------------------------------------------------------
// #name bound_block.h
// #description �{�X��Őݒu����o�E���h����I�u�W�F�N�g
// #make 2025/02/06�@�i��`��
// #update 2025/02/06
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------

#ifndef BOSS_BOUND_BLOCK_H
#define BOSS_BOUND_BLOCK_H
#include"include/box2d/box2d.h"
#include"1-1_boss_field_block.h"
#include<memory>




class boss_bound_block
{
public:
	boss_bound_block(b2Vec2 position, b2Vec2 size, b2Vec2 vec, Boss_Room_Level level,int texture_type);
	~boss_bound_block();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Player_jump();



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

	//�Z�b�g
	void SetJumpFlag(bool flag)
	{
		jump_flag = flag;
	}



private:

	int id;

	bool isUse=true;

	b2Body* m_body;

	b2Vec2 m_size;

	b2Vec2 vectol;

	bool jump_flag;

	Boss_Room_Level BossRoomLevel;

	int body_delete_cnt = 0;

	bool break_flag = false;

	int Change_Texture_Size_Frame;

	int Texture_type;

	std::unique_ptr<class ObjectData> m_objectData;  // ���j�[�N�|�C���^�[�ɕύX


};



#endif // BOSS_FIELD_BLOCK_H