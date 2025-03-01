//-----------------------------------------------------------------------------------------------------
// #name Item_Coin.h
// #description     coinアイテム
// #make 2024/12/28　永野義也
// #update 2024/12/28
// #comment 追加・修正予定

//----------------------------------------------------------------------------------------------------

#ifndef ITEM_COIN_H
#define ITEM_COIN_H

#include"include/box2d/box2d.h"

class ItemCoin
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	~ItemCoin();

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

	//これから消えるかどうかを取得
	bool	GetDestory() { return m_destory; }
	//これから消えるかどうかをセット
	void	SetDestory(bool flag) { m_destory = flag; }
	//アイテムがゲットされた時の処理
	void	Function();

	//	どっかの中間地に登録されたかどうかを取得
	bool	GetIfRegisteredToSavePoint() { return m_registered_to_save_point; }
	//　どっかの中間地に登録されたかどうかをセット
	void	SetIfRegisteredToSavePoint(bool flag) { m_registered_to_save_point = flag; }


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void DrawEffect();

private:
	// 各インスタンス固有の ID
	int m_ID;

	//アイテムのボディー
	b2Body* m_body;

	//リスポン用に保存
	b2Vec2 m_body_position;

	//リスポン用に保存
	float m_angle;

	//リスポン用に保存
	bool m_shape_polygon;


	//アイテムのサイズ（描画用）
	b2Vec2 m_size;

	//アイテムの透明度
	float m_Alpha;

	//消す予定なのかどうか
	bool	m_destory = false;

	//スプライトシートのカウントに使ってる
	int coin_effect_sheet_cnt = 0;


	//コインの発生条件に使っている
	int coin_effect_start_cnt = 0;


	b2Vec2 coin_effect_pos;

	//中間地に登録されたかどうか
	bool	m_registered_to_save_point = false;

};


#endif // !ITEM_COIN_H

