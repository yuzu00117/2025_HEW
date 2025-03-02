//-----------------------------------------------------------------------------------------------------
// #name Item_Jewel.h
// #description     ��΃A�C�e��
// #make 2025/1/24�@���j�S
// #update 2025/1/24
// #comment �ǉ��E�C���\��
//                  �A���J�[�������C�N���ꂽ��Ԃ̕�΂̌��ʂɃA���J�[�����鑬�x�A�b�v��ǉ�����\��
//----------------------------------------------------------------------------------------------------


#ifndef ITEM_JEWEL_H
#define	ITEM_JEWEL_H

#include"include/box2d/box2d.h"
#include"texture.h"

enum Jewel_Type
{
	BLUE,
	RED,
	YELLOW,
};

class ItemJewel
{
public:
	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	ItemJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool shape_polygon = true, float Alpha = 1.0f);
	~ItemJewel();

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
	

	//	�Q�[�W�։������Ă���r���Ȃ̂��ǂ������擾
	bool	GetIfCollecting() { return m_collecting; }
	//�@�Q�[�W�։������Ă���r���Ȃ̂��ǂ������Z�b�g
	void	SetIfCollecting(bool flag);

	//	�ǂ����̒��Ԓn�ɓo�^���ꂽ���ǂ������擾
	bool	GetIfRegisteredToSavePoint() { return m_registered_to_save_point; }
	//�@�ǂ����̒��Ԓn�ɓo�^���ꂽ���ǂ������Z�b�g
	void	SetIfRegisteredToSavePoint(bool flag) { m_registered_to_save_point = flag; }

	//�@�����v���C���[�ɃQ�b�g���ꂽ�����`�F�b�N
	bool	SearchIfJewelHaveGotByPlayer() { return m_get_by_player; }

	//���������ʔ��������ǂ������擾
	bool	GetIfFunctioned() { return m_functioned; }

	//���ꂩ������邩�ǂ������擾
	bool	GetDestory() { return m_destory; }
	//���ꂩ������邩�ǂ������Z�b�g
	void	SetDestory(bool flag) { m_destory = flag; }
	//�A�C�e�����Q�b�g���ꂽ���̏���
	void	Function();

	void SetEffectPosition()
	{
		get_effect_pos =m_body->GetPosition();
		jem_get_sheet_cnt = 1;
	}

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// �e�C���X�^���X�ŗL�� ID
	int m_ID;

	//�A�C�e���̃{�f�B�[
	b2Body* m_body;

	//���X�|���p�ɕۑ�
	b2Vec2 m_body_position;

	//���X�|���p�ɕۑ�
	bool m_shape_polygon;


	//���X�|���p�ɕۑ�
	float m_angle;


	//�A�C�e���̃T�C�Y�i�`��p�j
	b2Vec2 m_size;

	//�A�C�e���̓����x
	float m_Alpha;

	//�e�N�X�`��
	ID3D11ShaderResourceView* g_Texture;		    //��΂̃e�N�X�`��
	ID3D11ShaderResourceView* g_get_effect_texture; //��΂��擾�������̃G�t�F�N�g
	ID3D11ShaderResourceView* g_getting_effect_texture; //�擾���̃G�t�F�N�g
	ID3D11ShaderResourceView* g_using_effect_texture;  //�g�p�G�t�F�N�g


	//���ʔ��������̂�
	bool	m_functioned = false;

	//�����\��Ȃ̂��ǂ���
	bool	m_destory = false;

	//��������ǂ���
	bool	m_collecting = false;

	//�v���C���[�ɃQ�b�g���ꂽ���ǂ���
	bool	m_get_by_player = false;

	//���Ԓn�ɓo�^���ꂽ���ǂ���
	bool	m_registered_to_save_point = false;

	//����̌o�ߎ���
	float	m_collecting_time = 0.3f;

	//����J�n���̍��W�i�C�[�W���O�Ŏg���j
	b2Vec2	m_position_collecting_start;

	//������̍��W
	b2Vec2 m_position_while_collecting;

	//��΂̃^�C�v
	Jewel_Type	m_type;

	//�A�j���[�V����id(�G�t�F�N�g)
	int m_anim_id = 0;
	//�G�t�F�N�g�Đ����x����(���t���[���ňꖇ�G��i�߂邩)
	const int m_anim_speed = 2;
	int m_anim_count = 0;

	//��΂��Q�b�g���Ă�
	float jem_get_sheet_cnt;

	b2Vec2 get_effect_pos;

	//�擾�G�t�F�N�g�̕`��p
	int m_getting_anim_id = 0;
	//�d�l�G�t�F�N�g�̕`��p
	int m_use_anim_id = 0;
	int m_use_anim_count = 0;
	bool m_if_effect_using = false;
};

#endif // !ITEM_JEWELRY_H


