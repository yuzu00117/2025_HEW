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
#include<string>
#include<list>

enum SpiritType
{
	Spirit_L,
	Spirit_M,
	Spirit_S,
};

enum SpiritState
{
	Spirit_Idle,	//地面に着いている
	Spirit_Rising,	//上昇している
	Spirit_Falling,	//上昇後の落下（オブジェクトと離れた瞬間の座標まで落下）
	Spirit_Collecting,	//プレイヤーに回収されいている途中
	Spirit_Destory,		//これから消される予定
};

class ItemSpirit
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, float Alpha = 1.0f);
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


	//当たっているオブジェクトを追加
	void	AddCollidedObject(b2Body* object) { m_CollidedObject.push_back(object); }
	//直近まで当たっているオブジェクトが誰かを取得
	const b2Body* FindLeastCollidedObject() {
		return m_CollidedObject.back();
	}
	//さっきまで当たっていたオブジェクトを消す
	void	DeleteCollidedObject(b2Body* object) {
		if (!(m_CollidedObject.begin() == m_CollidedObject.end()) && m_state != Spirit_Collecting)
		{
			m_CollidedObject.remove_if([object](b2Body* p) { return p == object; });

			if (m_CollidedObject.size() == 0)
			{
				SetState(Spirit_Falling);
				//今離れた瞬間のソウルの座標を落下の終点にする
				m_Falling_to_position = m_body->GetPosition();
			}
		}
	}


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

	//ソウルの種類
	SpiritType m_type;

	//アニメーションパターンID
	int m_anim_id = 1;

	//今の状態
	SpiritState m_state = Spirit_Idle;

	//ソウルが今当たっているオブジェクト（或いは地面）
	std::list<b2Body*>m_CollidedObject;

	b2Vec2 m_Falling_to_position;	//どの座標まで落ちるか（落下状態の時に使う）
};

#endif // !ITEM_SPIRIT_H
