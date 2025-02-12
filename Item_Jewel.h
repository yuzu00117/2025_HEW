//-----------------------------------------------------------------------------------------------------
// #name Item_Jewel.h
// #description     宝石アイテム
// #make 2025/1/24　王泳心
// #update 2025/1/24
// #comment 追加・修正予定
//                  アンカーがリメイクされたら赤の宝石の効果にアンカー投げる速度アップを追加する予定
//----------------------------------------------------------------------------------------------------


#ifndef ITEM_JEWEL_H
#define	ITEM_JEWEL_H

#include"include/box2d/box2d.h"
#include"texture.h"

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
	

	//	ゲージへ回収されている途中なのかどうかを取得
	bool	GetIfCollecting() { return m_collecting; }
	//　ゲージへ回収されている途中なのかどうかをセット
	void	SetIfCollecting(bool flag);

	//今もう効果発揮下かどうかを取得
	bool	GetIfFunctioned() { return m_functioned; }

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

	//リスポン用に保存
	b2Vec2 m_body_position;

	//リスポン用に保存
	bool m_shape_polygon;


	//リスポン用に保存
	float m_angle;


	//アイテムのサイズ（描画用）
	b2Vec2 m_size;

	//アイテムの透明度
	float m_Alpha;

	//テクスチャ
	ID3D11ShaderResourceView* g_Texture;//アンカーのテクスチャ

	//効果発揮したのか
	bool	m_functioned = false;

	//消す予定なのかどうか
	bool	m_destory = false;

	//回収中かどうか
	bool	m_collecting = false;

	//回収の経過時間
	float	m_collecting_time = 0.3f;

	//回収開始時の座標（イージングで使う）
	b2Vec2	m_position_collecting_start;

	//回収時の座標
	b2Vec2 m_position_while_collecting;

	//宝石のタイプ
	Jewel_Type	m_type;

	//アニメーションid
	int m_anim_id = 0;
};

#endif // !ITEM_JEWELRY_H


