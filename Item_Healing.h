//----------------------------------------------------------------------------------------------------
// #name Item_Healing.h
// #description     回復アイテムヘッダーファイル
// #make 2025/02/25
// #update 2025/02/25 髙橋連
// #comment 追加・修正予定

//----------------------------------------------------------------------------------------------------

#ifndef ITEM_HEALING_H
#define ITEM_HEALING_H

#include"include/box2d/box2d.h"

class ItemHealing
{
private:
	int m_id;

	b2Body* m_body;
	b2Vec2 m_position;
	float m_angle;
	b2Vec2 m_size;
	float m_Alpha = 1;

	//消す予定なのかどうか
	bool m_destory = false;
	//スプライトシートのカウントに使ってる
	int m_effect_sheet_cnt = 0;
	//コインの発生条件に使っている
	int m_effect_start_cnt = 0;
	b2Vec2 m_effect_pos;

	const int m_heal_hp = 100;

public:
	ItemHealing(b2Vec2 position, b2Vec2 body_size, float angle);
	~ItemHealing() = default;

	//リスポン用
	void CreateBody();

	b2Body* GetBody() 
	{ 
		return m_body; 
	}
	void SetBody(b2Body* body) 
	{
		m_body = body;
	}

	int GetID() const 
	{ 
		return m_id; 
	}
	void SetID(int id) 
	{ 
		m_id = id; 
	}

	b2Vec2 GetSize() const 
	{ 
		return m_size;
	}
	void SetSize(b2Vec2 size) 
	{ 
		m_size = size;
	}

	bool	GetDestory() 
	{ 
		return m_destory;
	}
	void	SetDestory(bool flag) 
	{ 
		m_destory = flag;
	}
	//アイテムがゲットされた時の処理
	void Function();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void DrawEffect();

};


#endif // !ITEM_HEALING_H