//-----------------------------------------------------------------------------------------------------
// #name static_to_dynamic_block.h
// #description 岩のオブジェクトのCPP
// #make 2024/12/06　永野義也
// #update 2024/12/06
// #comment 追加・修正予定
//          ・アンカーをくっつけたらそのぶったいが静的なオブジェクトから動的なオブジェクトになる
//			・ヤシの実とか落石に作りたい
//----------------------------------------------------------------------------------------------------

#ifndef STATIC_TO_DYNAMIC_BLOCK_H
#define STATIC_TO_DYNAMIC_BLOCK_H

#include"include/box2d/box2d.h"
#include"sound.h"


enum collider_type_Box_or_Circle
{
	Box_collider,
	Circle_collider
};

class static_to_dynamic_block
{
public:

	static_to_dynamic_block(b2Vec2 Position, b2Vec2 size, collider_type_Box_or_Circle collider_type, int need_anchor_level,bool break_flag);
	//スポナーから生成される壊れるブロック用のコンストラクタ
	static_to_dynamic_block(b2Vec2 position, b2Vec2 body_size, int need_level, int id);
	~static_to_dynamic_block();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Change_dynamic();


	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//描画用にサイズを持たせておく
	b2Vec2 GetSize() const
	{
		return object_size;
	}

	void SetSize(b2Vec2 size) {
		object_size = size;
	}


	//bodyの習得
	b2Body* GetObjectBody()
	{
		return object_body;
	}

	void SetObjectBody(b2Body* body)
	{
		object_body = body;
	}

	//コライダーの形を記憶しておく
	collider_type_Box_or_Circle GetBox_or_Circle()
	{
		return box_or_circle;
	}

	void SetBox_or_Circle(collider_type_Box_or_Circle type)
	{
		box_or_circle = type;
	}


	bool Get_Change_Dynamic_flag()
	{
		return change_dynamic_flag;
	}

	void Set_Change_Dynamic_flag(bool flag)
	{
		change_dynamic_flag = flag;
	}


	void SetNowBreakBlock(bool flag)
	{
		Now_Break_Flag = flag;
	}

	bool	GetIfPulling() { return m_pulling; }
	void	SetIfPulling(bool flag) { m_pulling = flag; }


private:
	int id; // 各インスタンス固有の ID

	b2Body* object_body;

	b2Vec2 object_size;

	collider_type_Box_or_Circle box_or_circle;

	bool change_dynamic_flag;

	bool	m_pulling = false;


	bool Break_Flag;

	bool Now_Break_Flag=false;

	b2Vec2 prevVelocity;

	bool m_is_border = true;
	int m_need_level = 0;
	float m_border_alpha = 0;
	const float m_border_alpha_max = 0.75f;
	const float m_border_alpha_min = 0.0f;


	bool camera_shake_was = false;
};


#endif // !STATIC_TO_DYNAMIC_BLOCK_H
