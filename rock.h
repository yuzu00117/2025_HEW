//-----------------------------------------------------------------------------------------------------
// #name rock.h
// #description 岩のオブジェクトのヘッダー
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//			・あるなら右だけに倒れたいとかあったらやるかもねー
//----------------------------------------------------------------------------------------------------


#ifndef ROCK_H
#define ROCK_H

#include"include/box2d/box2d.h"




class rock
{
public:

	rock(b2Vec2 Position, float radius, int need_anchor_level);
	~rock();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_rock();


	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//描画用にサイズを持たせておく
	b2Vec2 GetRockSize() const
	{
		return m_Rock_size;
	}

	void SetRockSize(b2Vec2 size) {
		m_Rock_size = size;
	}


	//bodyの習得
	b2Body* GetObjectRockBody()
	{
		return Rock_body;
	}

	void SetObjectRockBody(b2Body* body)
	{
		Rock_body = body;
	}


	b2Vec2	GetPullingPower() { return m_pulling_power; }
	void	SetPullingPower_With_Multiple(b2Vec2 multiple) {
		m_pulling_power.x *= multiple.x;
		m_pulling_power.y *= multiple.y;
	}


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

	//引っ張る時の力
	b2Vec2 m_pulling_power;

	b2Body* Rock_body;

	b2Vec2 m_Rock_size;


	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;
};
#endif // !WOOD_H