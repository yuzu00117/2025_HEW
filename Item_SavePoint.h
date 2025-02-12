//-----------------------------------------------------------------------------------------------------
// #name Item_SavePoint.h
// #description �Z�[�u�|�C���g�̃w�b�_�[
// #make 2025/2/04�@���j�S
// #update 2025/2/04
// #comment �ǉ��E�C���\��
//          
//			
//----------------------------------------------------------------------------------------------------
#ifndef ITEM_SAVEPOINT_H
#define ITEM_SAVEPOINT_H

#include"include/box2d/box2d.h"

class ItemSavePoint
{
public:
	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	ItemSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	~ItemSavePoint();

	//���X�|���p
	void	CreateBody();

	//�{�f�B�[���擾
	b2Body* GetBody() { return m_body; }
	//�{�f�B�[���Z�b�g
	void SetBody(b2Body* body) { m_body = body; }


	// ID ���擾����
	int GetID() const { return m_ID; }
	// ID ���Z�b�g
	void SetID(int ID) { m_ID = ID; }

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSize() const { return m_size; }
	//�`��T�C�Y�Z�b�g
	void SetSize(b2Vec2 size) { m_size = size; }

	bool	GetIfPlayerPassed() { return m_player_passed; }
	void	SetPlayerPassed() { 
		if (!m_player_passed) {
			Function();
		}
		m_player_passed = true;
	}

	//�Z�[�u�|�C���g�ɓ��B�������̏���
	void	Function();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// �e�C���X�^���X�ŗL�� ID
	int m_ID;

	//�Z�[�u�|�C���g�̃{�f�B�[
	b2Body* m_body;

	//���X�|���p�ɕۑ�
	b2Vec2 m_body_position;

	//���X�|���p�ɕۑ�
	float m_angle;

	//���X�|���p�ɕۑ�
	bool m_shape_polygon;


	//�Z�[�u�|�C���g�̃T�C�Y�i�`��p�j
	b2Vec2 m_size;

	//�Z�[�u�|�C���g�̓����x
	float m_Alpha;

	//�v���C���[�����ʉ߂�����
	bool m_player_passed = false;
};

#endif // !ITEM_SAVEPOINT_H


