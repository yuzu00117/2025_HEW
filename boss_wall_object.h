//-----------------------------------------------------------------------------------------------------
// #name boss_wall_object.h
// #description �{�X��Ŏg���ǂ̃I�u�W�F�N�g
// #make 2025/02/13		�i��`��
// #update 2025/02/13
// #comment �ǉ��E�C���\��
//----------------------------------------------------------------------------------------------------


#ifndef BOSS_WALL_OBJECT_H
#define BOSS_WALL_OBJECT_H

#include"include/box2d/box2d.h"
#include"1-1_boss_field_block.h"
#include"sprite.h"
#include"texture.h"
#include<vector>

class Boss_Wall_Objcet
{
public:
	Boss_Wall_Objcet(b2Vec2 position, b2Vec2 size, int splitting_x, int splitting_y,ID3D11ShaderResourceView* g_Texture,bool left);
	~Boss_Wall_Objcet();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();


	void Destroy_Splitting(void);

	void DestroySplittedBodies(std::vector<b2Body*>& bodyList);


	void CreateAnchorPoint(void);

	void DeleteAnchorPoint(void);

	void ChangeBody(void);

	void WallPullling(void);

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


	bool GetrPullingFlag(void)
	{
		return pulling_flag;
	}

	void SetPullingFlag(bool flag)
	{
		pulling_flag = flag;
	}


private:

	bool isUse;

	int id;

	b2Vec2 m_pulling_power;

	b2Body* m_body;//���̃{�f�B

	b2Body* AnchorPoint_body;//�A���J�[�|�C���g�̃{�f�B

	b2Vec2 m_size;//�T�C�Y

	bool left_flag;//�A���J�[�|�C���g��\������ʒu

	int Splitting_x=10;//���̕�������

	int Splitting_y=16;//���Ă̕�������

	bool Splitting_Destroy_Flag = false;//���̃{�f�B��j�󂵂ĕ�������t���O

	bool Splitting_end = false;//�����I��

	int Destroy_Cnt = 0;

	int touch_ground_cnt = 0;

	bool pulling_flag = false;

	bool move_flag = false;

	int pulling_cnt;

	//�e�N�X�`��
	ID3D11ShaderResourceView* Texture;


	//�����o���o���ɂȂ����Ƃ��Ɏg��
	std::vector<b2Body*> boss_pillar_body_Splitting;
};

#endif // 
