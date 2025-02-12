//-----------------------------------------------------------------------------------------------------
// #name Item_SavePoint.h
// #description セーブポイントのヘッダー
// #make 2025/2/04　王泳心
// #update 2025/2/04
// #comment 追加・修正予定
//          
//			
//----------------------------------------------------------------------------------------------------
#ifndef ITEM_SAVEPOINT_H
#define ITEM_SAVEPOINT_H

#include"include/box2d/box2d.h"

class ItemSavePoint
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	~ItemSavePoint();

	//リスポン用
	void	CreateBody();

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

	bool	GetIfPlayerPassed() { return m_player_passed; }
	void	SetPlayerPassed() { 
		if (!m_player_passed) {
			Function();
		}
		m_player_passed = true;
	}

	//セーブポイントに到達した時の処理
	void	Function();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// 各インスタンス固有の ID
	int m_ID;

	//セーブポイントのボディー
	b2Body* m_body;

	//リスポン用に保存
	b2Vec2 m_body_position;

	//リスポン用に保存
	float m_angle;

	//リスポン用に保存
	bool m_shape_polygon;


	//セーブポイントのサイズ（描画用）
	b2Vec2 m_size;

	//セーブポイントの透明度
	float m_Alpha;

	//プレイヤーが一回通過したか
	bool m_player_passed = false;
};

#endif // !ITEM_SAVEPOINT_H


