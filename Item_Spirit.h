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

class ItemSpirit
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemSpirit(b2Vec2 position, b2Vec2 body_size, float angle, float recovery, bool shape_polygon = true, float Alpha = 1.0f);
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


	//回収されているかを取得
	bool	GetIfCollecting() { return m_collecting; }
	//回収されているかをセット
	void	SetIfCollecting(bool flag) { m_collecting = flag; }

	//これから消えるかどうかを取得
	bool	GetDestory() { return m_destory; }
	//これから消えるかどうかをセット
	void	SetDestory(bool flag) { m_destory = flag; }
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

	//消す予定なのかどうか
	bool	m_destory = false;

	//ソウル回復値
	float m_recovery;

	//プレイヤーに回収されているかどうか
	bool	m_collecting;

};

#endif // !ITEM_SPIRIT_H
