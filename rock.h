//-----------------------------------------------------------------------------------------------------
// #name rock.h
// #description ��̃I�u�W�F�N�g�̃w�b�_�[
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//			�E����Ȃ�E�����ɓ|�ꂽ���Ƃ����������邩���ˁ[
//----------------------------------------------------------------------------------------------------


#ifndef ROCK_H
#define ROCK_H

#include"include/box2d/box2d.h"
#include"sound.h"



class rock
{
public:

	rock(b2Vec2 Position, float radius, int need_anchor_level,bool left);
	~rock();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_rock();


	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetRockSize() const
	{
		return m_Rock_size;
	}

	void SetRockSize(b2Vec2 size) {
		m_Rock_size = size;
	}


	//body�̏K��
	b2Body* GetObjectRockBody()
	{
		return Rock_body;
	}

	void SetObjectRockBody(b2Body* body)
	{
		Rock_body = body;
	}

	bool	GetIfPulling() { return m_pulling; }
	void	SetIfPulling(bool flag) { m_pulling = flag; }

	b2Vec2	GetPullingPower() { return m_pulling_power; }
	void	SetPullingPower_With_Multiple(b2Vec2 multiple) {
		m_pulling_power.x *= multiple.x;
		m_pulling_power.y *= multiple.y;
	}


	///-----------------------------------------------------------------------------
	//�A���J�[�|�C���g


	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetAnchorPointSize() const
	{
		return m_AnchorPoint_size;
	}

	void SetAnchorPointSize(b2Vec2 size) {
		m_AnchorPoint_size = size;
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

private:
	int id; // �e�C���X�^���X�ŗL�� ID

	//�������鎞�̗�
	b2Vec2 m_pulling_power;

	b2Body* Rock_body;

	b2Vec2 m_Rock_size;


	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;

	bool	m_pulling = false;

	bool left_flag;

	bool m_is_border = true;
	int m_need_level = 0;
	float m_border_alpha = 0;
	const float m_border_alpha_max = 0.75f;
	const float m_border_alpha_min = 0.0f;

};
#endif // !WOOD_H