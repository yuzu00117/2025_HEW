//----------------------------------------------------------------------------------------------------
// #name Item_Healing.h
// #description     �񕜃A�C�e���w�b�_�[�t�@�C��
// #make 2025/02/25
// #update 2025/02/25 �����A
// #comment �ǉ��E�C���\��

//----------------------------------------------------------------------------------------------------

#ifndef ITEM_HEALING_H
#define ITEM_HEALING_H

#include"include/box2d/box2d.h"

class ItemHealing
{
private:
	int m_id;

	b2Body* m_body;
	b2Vec2 m_position;
	float m_angle;
	b2Vec2 m_size;
	float m_Alpha = 1;

	//�����\��Ȃ̂��ǂ���
	bool m_destory = false;
	//�X�v���C�g�V�[�g�̃J�E���g�Ɏg���Ă�
	int m_effect_sheet_cnt = 0;
	//�R�C���̔��������Ɏg���Ă���
	int m_effect_start_cnt = 0;
	b2Vec2 m_effect_pos;

	const int m_heal_hp = 100;

public:
	ItemHealing(b2Vec2 position, b2Vec2 body_size, float angle);
	~ItemHealing() = default;

	//���X�|���p
	void CreateBody();

	b2Body* GetBody() 
	{ 
		return m_body; 
	}
	void SetBody(b2Body* body) 
	{
		m_body = body;
	}

	int GetID() const 
	{ 
		return m_id; 
	}
	void SetID(int id) 
	{ 
		m_id = id; 
	}

	b2Vec2 GetSize() const 
	{ 
		return m_size;
	}
	void SetSize(b2Vec2 size) 
	{ 
		m_size = size;
	}

	bool	GetDestory() 
	{ 
		return m_destory;
	}
	void	SetDestory(bool flag) 
	{ 
		m_destory = flag;
	}
	//�A�C�e�����Q�b�g���ꂽ���̏���
	void Function();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void DrawEffect();

};


#endif // !ITEM_HEALING_H