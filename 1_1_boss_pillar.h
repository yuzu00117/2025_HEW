//-----------------------------------------------------------------------------------------------------
// #name 1_1_boss_pillar.h
// #description �{�X��ŗ��p���钌�̃I�u�W�F�N�g
// #make 2025/01/25		�i��`��
// #update 2025/01/25
// #comment �ǉ��E�C���\��
//----------------------------------------------------------------------------------------------------


#ifndef BOSS_PILIAR_H
#define BOSS_PILIAR_H

#include"include/box2d/box2d.h"
#include"1-1_boss_field_block.h"
#include<vector>
#include<memory>

class boss_pillar
{
public:
	boss_pillar(b2Vec2 position, b2Vec2 size, int splitting_x, int splitting_y,Boss_Room_Level level, int anchor_need_level);
	~boss_pillar();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();


	void Destroy_Splitting(void);

	

	void Pulling_pillar();

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

	//body�̏K��
	b2Body* GetObjectAnchorPointBody()
	{
		return AnchorPoint_body;
	}

	void SetObjectAnchorPointBody(b2Body* body)
	{
		AnchorPoint_body = body;
	}


	void SetSplitting_Destroy_Flag(bool flag)
	{
		Splitting_Destroy_Flag = flag;
	}



	int GetGroundTouchCnt(void)
	{
		return touch_ground_cnt;
	}


	void SetGroundTouchCnt(int cnt)
	{
		touch_ground_cnt = cnt;
	}

	b2Vec2	GetPullingPower() { return m_pulling_power; }
	void	SetPullingPower_With_Multiple(b2Vec2 multiple) {
		m_pulling_power.x *= multiple.x;
		m_pulling_power.y *= multiple.y;
	}


	void SetPullingFlag(bool flag)
	{
		pulling_flag = flag;
		m_is_border = false;
	}

private:

	bool isUse;

	int id;

	b2Vec2 m_pulling_power;

	b2Body* m_body;//���̃{�f�B

	b2Body* AnchorPoint_body;//�A���J�[�|�C���g�̃{�f�B

	b2Vec2 m_size;//�T�C�Y

	int Splitting_x;//���̕�������

	int Splitting_y;//���Ă̕�������

	bool Splitting_Destroy_Flag=false;//���̃{�f�B��j�󂵂ĕ�������t���O

	bool Splitting_end = false;//�����I��

	int Destroy_Cnt=0;

	int touch_ground_cnt = 0;

	bool pulling_flag=false;

	int angle_delete_cnt=0;


	b2Vec2 old_vec;

	Boss_Room_Level boss_room_level;


	bool m_is_border = true;
	int m_need_level = 0;
	float m_border_alpha = 0;
	const float m_border_alpha_max = 0.75f;
	const float m_border_alpha_min = 0.0f;

	std::unique_ptr<class ObjectData> m_objectData;  // ���j�[�N�|�C���^�[�ɕύX


	std::unique_ptr<class ObjectData> m_anchorData;  // ���j�[�N�|�C���^�[�ɕύX
};

#endif // !BOSS_PILLAR_H
