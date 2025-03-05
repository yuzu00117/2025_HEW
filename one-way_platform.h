//-----------------------------------------------------------------------------------------------------
// #name one-way_platform.h
// #description ����̃I�u�W�F�N�g�̃w�b�_�[
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//			�E����Ȃ�E�����ɓ|�ꂽ���Ƃ����������邩���ˁ[
//----------------------------------------------------------------------------------------------------


#ifndef ONE_WAY_PLATFORM
#define ONE_WAY_PLATFORM

#include"include/box2d/box2d.h"
#include<memory>




class one_way_platform
{
public:
	one_way_platform(b2Vec2 Postion, b2Vec2 local_Postion, b2Vec2 size,bool object_contact);
	~one_way_platform();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	


	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSize() const
	{
		return m_one_way_platform_size;
	}

	void SetSize(b2Vec2 size) {
		m_one_way_platform_size = size;
	}


	//body�̏K��
	b2Body* GetObject_one_way_platform_Body()
	{
		return one_way_platform_body;
	}

	void SetObject_one_way_platform_Body(b2Body* body)
	{
		one_way_platform_body = body;
	}


	b2Fixture* GetChangeFixture(void)
	{
		return change_fixture_list;
	}

	void SetChangeFixture(b2Fixture* fixture)
	{
		change_fixture_list = fixture;
	}


	void SetlocalPosition(b2Vec2 pos)
	{
		localpos = pos;
	}

	b2Vec2 GetlocalPosition(void)
	{
		return localpos;
	}
private:
	int id; // �e�C���X�^���X�ŗL�� ID

	b2Body* one_way_platform_body;

	b2Vec2 m_one_way_platform_size;

	b2Vec2 localpos;

	b2Fixture *change_fixture_list;

	std::unique_ptr<class ObjectData> m_objectData;  // ���j�[�N�|�C���^�[�ɕύX

};
#endif // !WOOD_H
