//-----------------------------------------------------------------------------------------------------
// #name boss_wall_object.h
// #description ボス戦で使う壁のオブジェクト
// #make 2025/02/13		永野義也
// #update 2025/02/13
// #comment 追加・修正予定
//----------------------------------------------------------------------------------------------------


#ifndef BOSS_WALL_OBJECT_H
#define BOSS_WALL_OBJECT_H

#include"include/box2d/box2d.h"
#include"1-1_boss_field_block.h"
#include"sprite.h"
#include"texture.h"
#include<vector>

class Boss_Wall_Objcet
{
public:
	Boss_Wall_Objcet(b2Vec2 position, b2Vec2 size, int splitting_x, int splitting_y,ID3D11ShaderResourceView* g_Texture,bool left);
	~Boss_Wall_Objcet();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();


	void Destroy_Splitting(void);

	void DestroySplittedBodies(std::vector<b2Body*>& bodyList);


	void CreateAnchorPoint(void);

	void DeleteAnchorPoint(void);

	void ChangeBody(void);

	void WallPullling(void);

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


	bool GetrPullingFlag(void)
	{
		return pulling_flag;
	}

	void SetPullingFlag(bool flag)
	{
		pulling_flag = flag;
	}


private:

	bool isUse;

	int id;

	b2Vec2 m_pulling_power;

	b2Body* m_body;//柱のボディ

	b2Body* AnchorPoint_body;//アンカーポイントのボディ

	b2Vec2 m_size;//サイズ

	bool left_flag;//アンカーポイントを表示する位置

	int Splitting_x=10;//横の分割すう

	int Splitting_y=16;//たての分割すう

	bool Splitting_Destroy_Flag = false;//柱のボディを破壊して分割するフラグ

	bool Splitting_end = false;//分割終了

	int Destroy_Cnt = 0;

	int touch_ground_cnt = 0;

	bool pulling_flag = false;

	bool move_flag = false;

	int pulling_cnt;

	//テクスチャ
	ID3D11ShaderResourceView* Texture;


	//柱がバラバラになったときに使う
	std::vector<b2Body*> boss_pillar_body_Splitting;
};

#endif // 
