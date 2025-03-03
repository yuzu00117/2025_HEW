//-----------------------------------------------------------------------------------------------------
// #name Item_DamageValue.h
// #description ダメージ表記
// #make 2025/2/27　王泳心
// #update 2025/2/27
// #comment 追加・修正予定
//      
//
// 
//----------------------------------------------------------------------------------------------------
#ifndef ITEM_DAMAGEVALUE_H
#define	ITEM_DAMAGEVALUE_H

#include"include/box2d/box2d.h"
#include <vector>
#include "texture.h"

//どんなやつのダメージなのか
enum DamageOwnerType
{
	DamageOwnerType_enemy,	//敵のダメージ
	DamageOwnerType_player,	//プレイヤーのダメージ
};

class ItemDamageValue
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// ownerはダメージを受けたやつのタイプを渡す
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemDamageValue(b2Vec2 position, b2Vec2 body_size, float angle, DamageOwnerType owner, int damage_value, float Alpha = 1.0f);
	~ItemDamageValue();

	//ボディーを取得
	b2Body* GetBody() { return m_body; }
	b2Body* GetBody() const { return m_body; }
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


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// 各インスタンス固有の ID
	int m_ID;

	//セーブポイントのボディー
	b2Body* m_body;

	//セーブポイントのサイズ（描画用）
	b2Vec2 m_size;

	//セーブポイントの透明度
	float m_Alpha;

	//どんなやつのダメージなのか
	DamageOwnerType m_owner;

	//ダメージ数値
	int	m_damage_value;

	//表示時間のカウントダウン
	int time_count = 60;

	//画像の横数
	int m_texture_total_cols;
	//画像の縦数
	int m_texture_total_rows;

	//桁のパターンID
	std::vector<int>pattern_ID;

	ID3D11ShaderResourceView* m_enemy_damage_Texture = NULL;//アンカーのテクスチャ
	ID3D11ShaderResourceView* m_player_damage_Texture = NULL;//アンカーのテクスチャ
};

#endif // !ITEM_DAMAGEVALUE_H
