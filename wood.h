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
	
	

	//描画用にサイズを持たせておく
	b2Vec2 GetStumpSize() const
	{
		return m_Stump_size;
	}

	void SetStumpSize(b2Vec2 size) {
		m_Stump_size = size;
	}
	//bodyの習得
	b2Body* GetObjectStumpBody()
	{
		return Stump_body;
	}

	void SetObjectStumpBody(b2Body* body)
	{
		Stump_body = body;
	}

	//木本体と切り株の間の溶接ジョイント取得
	b2Joint* GetWoodStumpJoint() 
	{
		return m_wood_stump_joint; 
	}

	void	SetWoodStumpJoint(b2Joint* joint)
	{ 
		m_wood_stump_joint = joint; 
	}


	bool	GetIfPulling() { return m_pulling; }
	void	SetIfPulling(bool flag) {
		m_pulling = flag;
		if (m_pulling)
		{
			m_destory_joint = true;	//切り株と本体のジョイントを消すためにフラグをオンにする
		}
	}

	//倒れる時ぶつかったオブジェクトのリストにオブジェクト追加
	void	Add_CollidedObjectWhenFalling_List(b2Vec2 position);

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


	b2Body* Stump_body;

	b2Vec2 m_Stump_size;

	b2Joint*	m_wood_stump_joint;

	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;

	bool	m_pulling = false;

	bool	m_destory_joint = false;	//切り株と本体のジョイントを消すかどうか

	//倒れる時ぶつかったオブジェクトの情報を入れるためのクラス
	class ObjectCollided_WhenFalling {
	public:
		b2Vec2	position;
		int	count_down_to_play_sound;
	};

	//倒れる時ぶつかったオブジェクトのlist
	std::list<ObjectCollided_WhenFalling*>object_collided_when_falling;

	//音源
	//----------------------------------------
	Sound_Manager m_sound_FalledDown = Object_Wood_Fall_Sound;	//倒れた時の音
};
#endif // !WOOD_H
