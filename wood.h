//-----------------------------------------------------------------------------------------------------
// #name wood.h
// #description 木のオブジェクトのヘッダー
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//			・あるなら右だけに倒れたいとかあったらやるかもねー
//----------------------------------------------------------------------------------------------------


#ifndef WOOD_H
#define WOOD_H

#include"include/box2d/box2d.h"
#include"sound.h"
#include"main.h"
#include<list>


class wood
{
public:
	wood(b2Vec2 position,b2Vec2 Woodsize,b2Vec2 AnchorPoint_size,int need_level);
	~wood();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_wood(b2Vec2 pullingpower);


	// ID を取得する
	int GetID() const {
		return id;
	}
	
	void SetID(int ID)
	{
		id = ID;
	}

	//描画用にサイズを持たせておく
	b2Vec2 GetWoodSize() const
	{
		return m_Wood_size;
	}

	void SetWoodSize(b2Vec2 size) {
		m_Wood_size = size;
	}


	//bodyの習得
	b2Body* GetObjectWoodBody()
	{
		return Wood_body;
	}

	void SetObjectWoodBody(b2Body* body)
	{
		Wood_body = body;
	}


	bool	GetIfPulling() { return m_pulling; }
	void	SetIfPulling(bool flag) { m_pulling = flag; }

	int		GetPullingTime() { return m_pulling_time; }
	void	SetPullingTime(int time) { m_pulling_time = time; }


	void	Add_CollidedObjectWhenFalling_List(b2Vec2 position, ObjectType type);

	
	/*	void	FalledDownSound(ObjectType object) {
		if (object != NULL_object)
		{
			app_atomex_start(Player_Jump_Sound);
		}
		if (object == NULL_object && m_pulling_time > 60) {
			app_atomex_start(Player_Jump_Sound);
			m_pulling_time = 0;
		}
	}*/

	///-----------------------------------------------------------------------------
	//アンカーポイント


	//描画用にサイズを持たせておく
	b2Vec2 GetAnchorPointSize() const
	{
		return m_AnchorPoint_size;
	}

	void SetAnchorPointSize(b2Vec2 size) {
		m_AnchorPoint_size = size;
	}

	//bodyの習得
	b2Body* GetObjectAnchorPointBody()
	{
		return AnchorPoint_body;
	}

	void SetObjectAnchorPointBody(b2Body* body)
	{
		AnchorPoint_body = body;
	}

private:
	int id; // 各インスタンス固有の ID

	b2Body* Wood_body;

	b2Vec2 m_Wood_size;


	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;

	bool	m_pulling = false;

	int	m_pulling_time = 0;

	class ObjectCollided_WhenFalling {
	public:
		b2Vec2	position;
		ObjectType type;
		int	count_down_to_play_sound;
	};

	std::list<ObjectCollided_WhenFalling*>object_collided_when_falling;
};
#endif // !WOOD_H
