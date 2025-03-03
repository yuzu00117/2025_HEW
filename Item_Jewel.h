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

	//	どっかの中間地に登録されたかどうかを取得
	bool	GetIfRegisteredToSavePoint() { return m_registered_to_save_point; }
	//　どっかの中間地に登録されたかどうかをセット
	void	SetIfRegisteredToSavePoint(bool flag) { m_registered_to_save_point = flag; }

	//　もうプレイヤーにゲットされたかをチェック
	bool	SearchIfJewelHaveGotByPlayer() { return m_get_by_player; }

	//今もう効果発揮下かどうかを取得
	bool	GetIfFunctioned() { return m_functioned; }

	//これから消えるかどうかを取得
	bool	GetDestory() { return m_destory; }
	//これから消えるかどうかをセット
	void	SetDestory(bool flag) { m_destory = flag; }
	//アイテムがゲットされた時の処理
	void	Function();

	void SetEffectPosition()
	{
		get_effect_pos =m_body->GetPosition();
		jem_get_sheet_cnt = 1;
	}

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
	ID3D11ShaderResourceView* g_Texture;		    //宝石のテクスチャ
	ID3D11ShaderResourceView* g_get_effect_texture; //宝石を取得した時のエフェクト
	ID3D11ShaderResourceView* g_getting_effect_texture; //取得中のエフェクト
	ID3D11ShaderResourceView* g_using_effect_texture;  //使用エフェクト


	//効果発揮したのか
	bool	m_functioned = false;

	//消す予定なのかどうか
	bool	m_destory = false;

	//回収中かどうか
	bool	m_collecting = false;

	//プレイヤーにゲットされたかどうか
	bool	m_get_by_player = false;

	//中間地に登録されたかどうか
	bool	m_registered_to_save_point = false;

	//回収の経過時間
	float	m_collecting_time = 0.3f;

	//回収開始時の座標（イージングで使う）
	b2Vec2	m_position_collecting_start;

	//回収時の座標
	b2Vec2 m_position_while_collecting;

	//宝石のタイプ
	Jewel_Type	m_type;

	//アニメーションid(エフェクト)
	int m_anim_id = 0;
	//エフェクト再生速度調整(何フレームで一枚絵を進めるか)
	const int m_anim_speed = 2;
	int m_anim_count = 0;

	//宝石をゲットしてる
	float jem_get_sheet_cnt;

	b2Vec2 get_effect_pos;

	//取得エフェクトの描画用
	int m_getting_anim_id = 0;
	//仕様エフェクトの描画用
	int m_use_anim_id = 0;
	int m_use_anim_count = 0;
	bool m_if_effect_using = false;
};

#endif // !ITEM_JEWELRY_H


