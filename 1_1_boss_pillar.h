//-----------------------------------------------------------------------------------------------------
// #name 1_1_boss_pillar.h
// #description ボス戦で利用する柱のオブジェクト
// #make 2025/01/25		永野義也
// #update 2025/01/25
// #comment 追加・修正予定
//----------------------------------------------------------------------------------------------------


#ifndef BOSS_PILIAR_H
#define BOSS_PILIAR_H

#include"include/box2d/box2d.h"
#include"1-1_boss_field_block.h"
#include<vector>
#include<memory>

class boss_pillar
{
public:
	boss_pillar(b2Vec2 position, b2Vec2 size, int splitting_x, int splitting_y,Boss_Room_Level level, int anchor_need_level);
	~boss_pillar();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();


	void Destroy_Splitting(void);

	

	void Pulling_pillar();

	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}


	//ボディを取得
	b2Body* GetBody(void)
	{
		return m_body;
	}

	//ボディをセット
	void SetBody(b2Body* body)
	{
		m_body = body;
	}



	//サイズを取得
	b2Vec2 GetSize(void)
	{
		return m_size;
	}
	//サイズをセット
	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}

	//bodyの習得
	b2Body* GetObjectAnchorPointBody()
	{
		return AnchorPoint_body;
	}

	void SetObjectAnchorPointBody(b2Body* body)
	{
		AnchorPoint_body = body;
	}


	void SetSplitting_Destroy_Flag(bool flag)
	{
		Splitting_Destroy_Flag = flag;
	}



	int GetGroundTouchCnt(void)
	{
		return touch_ground_cnt;
	}


	void SetGroundTouchCnt(int cnt)
	{
		touch_ground_cnt = cnt;
	}

	b2Vec2	GetPullingPower() { return m_pulling_power; }
	void	SetPullingPower_With_Multiple(b2Vec2 multiple) {
		m_pulling_power.x *= multiple.x;
		m_pulling_power.y *= multiple.y;
	}


	void SetPullingFlag(bool flag)
	{
		pulling_flag = flag;
		m_is_border = false;
	}

private:

	bool isUse;

	int id;

	b2Vec2 m_pulling_power;

	b2Body* m_body;//柱のボディ

	b2Body* AnchorPoint_body;//アンカーポイントのボディ

	b2Vec2 m_size;//サイズ

	int Splitting_x;//横の分割すう

	int Splitting_y;//たての分割すう

	bool Splitting_Destroy_Flag=false;//柱のボディを破壊して分割するフラグ

	bool Splitting_end = false;//分割終了

	int Destroy_Cnt=0;

	int touch_ground_cnt = 0;

	bool pulling_flag=false;

	int angle_delete_cnt=0;


	b2Vec2 old_vec;

	Boss_Room_Level boss_room_level;


	bool m_is_border = true;
	int m_need_level = 0;
	float m_border_alpha = 0;
	const float m_border_alpha_max = 0.75f;
	const float m_border_alpha_min = 0.0f;

	std::unique_ptr<class ObjectData> m_objectData;  // ユニークポインターに変更


	std::unique_ptr<class ObjectData> m_anchorData;  // ユニークポインターに変更
};

#endif // !BOSS_PILLAR_H
