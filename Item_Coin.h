//-----------------------------------------------------------------------------------------------------
// #name Item_Coin.h
// #description     coin�A�C�e��
// #make 2024/12/28�@�i��`��
// #update 2024/12/28
// #comment �ǉ��E�C���\��

//----------------------------------------------------------------------------------------------------

#ifndef ITEM_COIN_H
#define ITEM_COIN_H

#include"include/box2d/box2d.h"

class ItemCoin
{
public:
	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	ItemCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	~ItemCoin();

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

	//���ꂩ������邩�ǂ������擾
	bool	GetDestory() { return m_destory; }
	//���ꂩ������邩�ǂ������Z�b�g
	void	SetDestory(bool flag) { m_destory = flag; }
	//�A�C�e�����Q�b�g���ꂽ���̏���
	void	Function();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// �e�C���X�^���X�ŗL�� ID
	int m_ID;

	//�A�C�e���̃{�f�B�[
	b2Body* m_body;

	//�A�C�e���̃T�C�Y�i�`��p�j
	b2Vec2 m_size;

	//�A�C�e���̓����x
	float m_Alpha;

	//�����\��Ȃ̂��ǂ���
	bool	m_destory = false;

};


#endif // !ITEM_COIN_H

