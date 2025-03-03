//-----------------------------------------------------------------------------------------------------
// #name Item_DamageValue.h
// #description �_���[�W�\�L
// #make 2025/2/27�@���j�S
// #update 2025/2/27
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------
#ifndef ITEM_DAMAGEVALUE_H
#define	ITEM_DAMAGEVALUE_H

#include"include/box2d/box2d.h"
#include <vector>
#include "texture.h"

//�ǂ�Ȃ�̃_���[�W�Ȃ̂�
enum DamageOwnerType
{
	DamageOwnerType_enemy,	//�G�̃_���[�W
	DamageOwnerType_player,	//�v���C���[�̃_���[�W
};

class ItemDamageValue
{
public:
	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// owner�̓_���[�W���󂯂���̃^�C�v��n��
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	ItemDamageValue(b2Vec2 position, b2Vec2 body_size, float angle, DamageOwnerType owner, int damage_value, float Alpha = 1.0f);
	~ItemDamageValue();

	//�{�f�B�[���擾
	b2Body* GetBody() { return m_body; }
	b2Body* GetBody() const { return m_body; }
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


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// �e�C���X�^���X�ŗL�� ID
	int m_ID;

	//�Z�[�u�|�C���g�̃{�f�B�[
	b2Body* m_body;

	//�Z�[�u�|�C���g�̃T�C�Y�i�`��p�j
	b2Vec2 m_size;

	//�Z�[�u�|�C���g�̓����x
	float m_Alpha;

	//�ǂ�Ȃ�̃_���[�W�Ȃ̂�
	DamageOwnerType m_owner;

	//�_���[�W���l
	int	m_damage_value;

	//�\�����Ԃ̃J�E���g�_�E��
	int time_count = 60;

	//�摜�̉���
	int m_texture_total_cols;
	//�摜�̏c��
	int m_texture_total_rows;

	//���̃p�^�[��ID
	std::vector<int>pattern_ID;

	ID3D11ShaderResourceView* m_enemy_damage_Texture = NULL;//�A���J�[�̃e�N�X�`��
	ID3D11ShaderResourceView* m_player_damage_Texture = NULL;//�A���J�[�̃e�N�X�`��
};

#endif // !ITEM_DAMAGEVALUE_H
