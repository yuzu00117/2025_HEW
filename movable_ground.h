//-----------------------------------------------------------------------------------------------------
// #name movable_ground.h
// #description ��������鏰�̃w�b�_�[
// #make 2024/12/26�@���j�S
// #update 2024/12/26
// #comment �ǉ��E�C���\��
//         
//----------------------------------------------------------------------------------------------------
#pragma once

#ifndef MOVABLE_GROUND_H
#define MOVABLE_GROUND_H

#include <iostream>
#include <list>
#include<algorithm>
#include"include/box2d/box2d.h"
#include"enemy_static.h"
#include"enemy_dynamic.h"


class movable_ground
{
public:
	movable_ground(b2Vec2 Position, b2Vec2 Ground_size, b2Vec2 AnchorPoint_size, int need_level);
	~movable_ground();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_ground(b2Vec2 pullingpower);


	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetGroundSize() const
	{
		return m_Ground_size;
	}

	void SetGroundSize(b2Vec2 size) {
		m_Ground_size = size;
	}


	//body�̏K��
	b2Body* GetObjectGroundBody()
	{
		return Ground_body;
	}

	void SetObjectGroundBody(b2Body* body)
	{
		Ground_body = body;
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

	//�����������Ă���̂����擾
	bool	GetIfPulling() {
		return pulling;
	}

	//�����������Ă���̂����Z�b�g
	void	SetIfPulling(bool flag) {
		pulling = flag;
	}

	void	AddContactedEnemyList(EnemyStatic* enemy)
	{
		enemy_static.push_back(enemy);
	}
	void	AddContactedEnemyList(EnemyDynamic* enemy)
	{
		enemy_dynamic.push_back(enemy);
	}

	void	DeleteContactedEnemyList(EnemyStatic* enemy)
	{
		auto it = enemy_static.begin();
		it = std::find(enemy_static.begin(), enemy_static.end(), enemy);
		if (it != enemy_static.end() && *it == enemy)
		{
			enemy_static.erase(it);
		}
	}

	void	DeleteContactedEnemyList(EnemyDynamic* enemy)
	{
		auto it = enemy_dynamic.begin();
		it = std::find(enemy_dynamic.begin(), enemy_dynamic.end(), enemy);
		if (it != enemy_dynamic.end() && *it == enemy)
		{
			enemy_dynamic.erase(it);
		}
	}

private:
	int id; // �e�C���X�^���X�ŗL�� ID

	bool pulling;
	b2Vec2	add_force;
	std::list<EnemyStatic*>enemy_static;
	std::list<EnemyDynamic*>enemy_dynamic;

	b2Body* Ground_body;

	b2Vec2 m_Ground_size;


	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;

};

#endif // !MOVABLE_GROUND_H

