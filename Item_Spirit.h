//-----------------------------------------------------------------------------------------------------
// #name Item_Spirit.h
// #description		ソウル（敵が落とすアイテム）
// #make 2024/12/28　王泳心
// #update 2024/12/28
// #comment 追加・修正予定
//      
//
// 
//----------------------------------------------------------------------------------------------------

#ifndef ITEM_SPIRIT_H
#define	ITEM_SPIRIT_H

#include"include/box2d/box2d.h"

enum SpiritState
{
	Spirit_OnGround,
	Spirit_Falling,
	Spirit_Rising,
	Spirit_Collecting,
	Spirit_Destory,
};

class ItemSpirit
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemSpirit(b2Vec2 position, b2Vec2 body_size, float angle, float recovery, float Alpha = 1.0f);
	~ItemSpirit();

	//ボディーを取得
	b2Body* GetBody() { return m_body; }
	//ボディーをセット
	void SetBody(b2Body* body) { m_body = body; }


	// ID を取得する
	int GetID() const { return m_ID; }
	// ID をセット
	void SetID(int ID) { m_ID = ID; }

	//描画用にサイズを持たせておく
	b2Vec2 GetSize() const { return m_size; }
	//描画サイズセット
	void SetSize(b2Vec2 size) { m_size = size; }

	//今の状態を取得
	SpiritState GetState() { return m_state; }
	//今の状態をセット
	void	SetState(SpiritState state);

	//直近まで当たっているオブジェクトが誰かを取得
	const b2Body*	GetRecentCollidedObject() { return m_recent_collided_object; }
	//直近まで当たっているオブジェクトをセット
	void	SetRecentCollidedObject(b2Body* object) { m_recent_collided_object = object; }

	//今プレイヤーと当たっているのかを取得
	bool	GetIfCollidedPlayer() { return m_collided_player; }
	//今プレイヤーと当たっているのかをセット
	void	SetIfCollidedPlayer(bool flag) { m_collided_player = flag; }

	//アイテムがゲットされた時の処理
	void	Function();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// 各インスタンス固有の ID
	int m_ID;

	//アイテムのボディー
	b2Body* m_body;

	//アイテムのサイズ（描画用）
	b2Vec2 m_size;

	//アイテムの透明度
	float m_Alpha;

	//ソウル回復値
	float m_recovery;

	//今の状態
	SpiritState m_state = Spirit_Falling;

	//直近まで重なっているオブジェクト（あるいは地面）
	const b2Body* m_recent_collided_object = nullptr;

	bool	m_collided_player;
};

#endif // !ITEM_SPIRIT_H
