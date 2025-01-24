//-----------------------------------------------------------------------------------------------------
// #name Item_Jewel.h
// #description     宝石アイテム
// #make 2025/1/24　王泳心
// #update 2025/1/24
// #comment 追加・修正予定

//----------------------------------------------------------------------------------------------------


#ifndef ITEM_JEWEL_H
#define	ITEM_JEWEL_H

#include"include/box2d/box2d.h"

enum Jewel_Type
{
	BLUE,
	RED,
	YELLOW,
};

class ItemJewel
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool shape_polygon = true, float Alpha = 1.0f);
	~ItemJewel();

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

	Jewel_Type	m_type;


};

#endif // !ITEM_JEWELRY_H


