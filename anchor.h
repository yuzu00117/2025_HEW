//-----------------------------------------------------------------------------------------------------
// #name anchor.h
// #description �A���J�[�̏���
// #make 2024/11/22 �i��`��
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E�A���J�[�̏����͂ЂƂ܂�����
//			�E�`�F�[���̏����͋����͂������ǁ@�`��p�ɕύX���K�v
//----------------------------------------------------------------------------------------------------
#ifndef ANCHOR_H
#define ANCHOR_H


enum AnchorState        //player.cpp�Ȃ���update�ŊǗ�����ۂɗ��p���Ă���
{
	Nonexistent_state,	//���݂��Ȃ����
	Create_wait_draw_cnt_state,//draw_cnt�p�̒���
	Create_wait_state, //�쐬����Ƃ��̃��[�V�����܂�
	Create_state,		//�쐬���Ă�����
	Throwing_state,		//�����Ă����ԁ@ Throw���Ăяo��
	Connected_state,	//�������Ă�����
	Pulling_state,		//���������Ă�����
	Deleting_state,


	WaitCreateNormalAttackDraw_cnt_state,//�ʏ�U����Drawstate�Ǘ��p
	WaitCraateNormalAttack_state,//�ʏ�U���̔����O�t���[������̂��߁@45�t���[��
	CreateNormalAttack_state,//�ʏ�U�����쐬������
	NowAttackngNormalAttack,//�ʏ�U���̍U����
	DeletingNormaklAttack_state,//�폜������

};


class Anchor
{
public:
	Anchor();//�R���X�g���N�^�ł͐������Ȃ�
	~Anchor();

	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();


	static void DrawChain();

	/**
	* @brief �A���J�[�𓊂��鏈��
	* �A���J�[���g�ƖڕW�ƂȂ���W���擾���āA�ڕW�̍��W�ɔ�΂�
	*/
	static void ThrowAnchorToAP();

	/**
	 * @brief �A���J�[����������
	 */
	static void PullingAnchor(void);

	/**
	 * @brief �A���J�[�𐶐�����
	 * @param size ��������A���J�[�̑傫��
	 */
	static void CreateAnchor(b2Vec2 size);



	/**
	 * @brief �A���J�[�̃{�f�B�𐶐�����
	 * @param anchor_size �������邷��A���J�[�傫��
	 */
	void CreateAnchorBody(b2Vec2 anchor_size);


	/**
	 * @brief �A���J�[�̃{�f�B���폜����
	 */
	void DestroyAnchorBody();//Body���폜����

	/**
	 * @brief �A���J�[���폜����
	 */
	static void DeleteAnchor();

	/**
	 * @brief  �Ԃ��������]�W���C���g��t����
	 */
	static void CreateRotateJoint();

	static void DeleteRotateJoint();


	/*
		
	*/
	static void CreateNormalAttack(b2Vec2 anchor_size, bool right);

	/**
	 * @brief  �ʏ�U�������邽�߂̃{�f�B������
	 * @param anchor_size �@�U���͈�
	 * @param right �@�W������������
	 */
	void CreateNormalAttackAnchorBody(b2Vec2 anchor_size,bool right);


	static void UpdateNormalAttack();

	static void DrawNormalAttack();


	static void DeleteNormalAttackAnchor();

	void DeleteNormalAttackAnchorBody();
	 






	b2Body* GetAnchorBody(void)
	{
		return m_body;
	}

	void SetAnchorBody(b2Body* anchor_body)
	{
		m_body = anchor_body;
	}


	b2Body* GetNormalAttackAnchorBody(void)
	{
		return m_normal_attack_body;
	}

	void SetNormalAttackAnchorBody(b2Body* anchor_normal_attack_body)
	{
		m_normal_attack_body = anchor_normal_attack_body;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSize() const
	{
		return m_anchor_size;
	}

	void SetSize(b2Vec2 size)
	{
		m_anchor_size = size;
	}



	//chain�̕`��p�̃T�C�Y
	b2Vec2 GetChainSize()const
	{
		return m_chain_size;
	}

	void SetChainSize(b2Vec2 size)
	{
		m_chain_size = size;
	}

	static void SetAnchorCreateJointFlag(bool flag);
	static bool GetAnchorCreateJointFlag();
	
	static void SetAnchorState(AnchorState state);
	static AnchorState GetAnchorState();

private:
	//�v���C���[��Body������
	b2Body* m_body;

	b2Body* m_normal_attack_body;

	b2Vec2 m_anchor_size;

	b2Vec2 m_chain_size;

	bool m_isAnchorCreated = false; // �{�f�B����������Ă��邩�Ǘ�

	static bool anchor_create_joint_flag;


	static AnchorState now_anchor_state;
};





#endif // !ANCHOR_H

