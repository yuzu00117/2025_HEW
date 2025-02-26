//-----------------------------------------------------------------------------------------------------
// #name Item_Barrier.h
// #description バリア
// #make 2025/2/26　王泳心
// #update 2025/2/26
// #comment 追加・修正予定
//      
//
// 
//----------------------------------------------------------------------------------------------------
#ifndef ITEM_BARRIER_H
#define ITEM_BARRIER_H

#include"include/box2d/box2d.h"


enum BarrierState
{
	Barrier_Start,		// バリアを張る
	Barrier_Loop,		// バリアが持続
	Barrier_Break,	// バリアが破壊された
};

class ItemBarrier
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemBarrier(b2Vec2 position, b2Vec2 body_size, float angle, const b2Body* owner_body, float Alpha = 1.0f);
	~ItemBarrier();

	//ボディーを取得
	b2Body* GetBody() { return m_body; }
	b2Body* GetBody() const { return m_body; }
	//ボディーをセット
	void SetBody(b2Body* body) { m_body = body; }


	// ID を取得する
	int GetID() const { return m_ID; }
	// ID をセット
	void SetID(int ID) { m_ID = ID; }

	//　バリアを張った本人を取得
	const b2Body* GetOwnerBody() { return m_owner; }

	//描画用にサイズを持たせておく
	b2Vec2 GetSize() const { return m_size; }
	//描画サイズセット
	void SetSize(b2Vec2 size) { m_size = size; }


	//バリアの状態を取得
	BarrierState GetState() { return m_state; }
	//バリアの状態をセット
	void SetState(BarrierState state){
		m_state = state;
		m_effect_cnt = 0;
		switch (m_state)
		{
		case Barrier_Start:
			m_texture_total_cols = 8;
			m_texture_total_rows = 6;
			break;
		case Barrier_Loop:
			m_texture_total_cols = 8;
			m_texture_total_rows = 6;
			break;
		case Barrier_Break:
			m_texture_total_cols = 5;
			m_texture_total_rows = 4;
			break;
		default:
			break;
		}
	}

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// 各インスタンス固有の ID
	int m_ID;

	//セーブポイントのボディー
	b2Body* m_body;

	//エフェクトを表示する
	float m_effect_cnt;

	//セーブポイントのサイズ（描画用）
	b2Vec2 m_size;

	//セーブポイントの透明度
	float m_Alpha;

	BarrierState m_state;

	//バリアを張った本人のbody情報
	const b2Body* m_owner;

	//画像の縦数
	int m_texture_total_cols;
	//画像の横数
	int m_texture_total_rows;
};


#endif // !ITEM_BARRIER_H

