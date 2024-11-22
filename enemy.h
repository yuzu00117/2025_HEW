//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description ���I�A�ÓI�G�l�~�[�̌p�����A�G�l�~�[�N���X�̃w�b�_�[�t�@�C��
// #make 2024/11/19
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E�v���C���[�ƏՓ˂ő̗͂����֐��m�ǉ�
//			�E�|�ꂽ�I�u�W�F�N�g�Ɋ������܂�ď����鏈���m�ǉ�
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_H
#define ENEMY_H

#include"world_box2d.h"

class Enemy
{
private:
	//�G�l�~�[��Body������
	b2Body* m_body;

	//�`��p��Body�̃T�C�Y�����Ă����@
	b2Vec2 m_size;
public:
	Enemy() = default;
	Enemy(b2Vec2 body_size, b2Vec2 position, bool dynamic);
	virtual ~Enemy() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize();

	b2Body* GetEnemyBody()
	{
		return m_body;
	}
	void SetEnemyBody(b2Body* body)
	{
		m_body = body;
	}

	b2Vec2 GetEnemySize()
	{
		return m_size;
	}
	void SetEnemySize(b2Vec2 size)
	{
		m_size = size;
	}
	
	//�����ł��ꔻ��t����
};

#endif	//ENEMY_H