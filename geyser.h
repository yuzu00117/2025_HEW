//-----------------------------------------------------------------------------------------------------
// #name geyser.h
// #description 間欠泉のヘッダー
// #make 2024/12/27　　永野義也
// #update 2024/12/27
// #comment 追加・修正予定
//                      ・特になし
//           
//----------------------------------------------------------------------------------------------------

#ifndef GEYSER_H
#define GEYSER_H

#include"include/box2d/box2d.h"

class geyser
{
public:

	geyser(b2Vec2 GeyserPosition, b2Vec2 GeyserSize, b2Vec2 RangeFlyWaterSize);
	~geyser();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Destroy_Joint();

	void PullingOnRock();

	void JumpPlayer();


	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}



	//間欠泉bodyの習得
	b2Body* GetGeyserBody()
	{
		return geyser_body;
	}
	void SetGeyserBody(b2Body* body)
	{
		geyser_body = body;
	}

	//間欠泉にサイズを持たせておく
	b2Vec2 GetGeyserSize() const
	{
		return geyser_size;
	}

	void SetGeyserSize(b2Vec2 size) {
		geyser_size = size;
	}

	


	//間欠泉の水の飛ぶ範囲の設定
	b2Vec2 GetRangeFlyWaterSize() const
	{
		return range_fly_water_size;
	}

	void SetRangeFlyWaterSize(b2Vec2 size) {
		range_fly_water_size = size;
	}





	//間欠泉の上に乗ってる岩のボディ
	b2Body* GetGeyserOnRockBody()
	{
		return geyser_on_rock_body;
	}

	void SetGeyserOnRockBody(b2Body* body)
	{
		geyser_on_rock_body = body;
	}


	// 間欠泉の上に乗ってる岩のサイズ
	b2Vec2 GetGeyserOnRockSize() const
	{
		return geyser_on_rock_size;
	}

	void SetGeyserOnRockSize(b2Vec2 size)
	{
		geyser_on_rock_size = size;
	}


	bool GetFlag()const
	{
		return water_flag;
	}

	void SetFlag(bool flag)
	{
		water_flag = flag;
	}

private:
	int id; // 各インスタンス固有の ID

	b2Body* geyser_body;//間欠泉のボディ

	b2Vec2 geyser_size;//間欠泉のサイズ

	b2Vec2 range_fly_water_size;//間欠泉の水が飛ぶ範囲


	b2Body* geyser_on_rock_body;//間欠泉に乗ってる岩のボディ

	b2Vec2 geyser_on_rock_size;//間欠泉に乗ってる岩のサイズ


	bool water_flag;//水がプレイヤーとふれている

	float easing_rate;




};


#endif //