//-----------------------------------------------------------------------------------------------------
// #name wood.h
// #description 木のオブジェクトのヘッダー
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//			・あるなら右だけに倒れたいとかあったらやるかもねー
//----------------------------------------------------------------------------------------------------


#ifndef WOOD_H
#define WOOD_H

#include"include/box2d/box2d.h"
#include"sound.h"
#include<list>

enum Wood_State
{
	Wood_Idle,
	Wood_Pulling,
	Wood_Falling,
	Wood_HitObject,
};


class wood
{
public:
	wood(b2Vec2 position,b2Vec2 Woodsize,b2Vec2 AnchorPoint_size,int need_level);
	~wood();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_wood(b2Vec2 pullingpower);


	// ID を取得する
	int GetID() const {
		return id;
	}
	
	void SetID(int ID)
	{
		id = ID;
	}

	//描画用にサイズを持たせておく
	b2Vec2 GetWoodSize() const
	{
		return m_Wood_size;
	}

	void SetWoodSize(b2Vec2 size) {
		m_Wood_size = size;
	}


	//bodyの習得
	b2Body* GetObjectWoodBody()
	{
		return Wood_body;
	}

	void SetObjectWoodBody(b2Body* body)
	{
		Wood_body = body;
	}


	bool	GetIfPulling() {
		if (m_state == Wood_Pulling)return true;
		else return false;
	}
	void	SetIfPulling(bool flag) {
		if (flag)
		{
			SetState(Wood_Pulling);
			angle_when_pulling_start = Wood_body->GetAngle();
		}
		else SetState(Wood_Idle);
	}

	//	木の状態をセット
	void	SetState(Wood_State state);

	///-----------------------------------------------------------------------------
	//アンカーポイント


	//描画用にサイズを持たせておく
	b2Vec2 GetAnchorPointSize() const
	{
		return m_AnchorPoint_size;
	}

	void SetAnchorPointSize(b2Vec2 size) {
		m_AnchorPoint_size = size;
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

private:
	int id; // 各インスタンス固有の ID

	b2Body* Wood_body;

	b2Vec2 m_Wood_size;


	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;

	Wood_State	m_state = Wood_Idle;

	int		start_stop_sound_count = 120;	//ゲーム開始直後木が地面まで落ちる時音鳴らさないためのカウントダウン

	float	angle_when_pulling_start = 0;	//引っ張り始める時や音鳴らされた直後の木の回転角度を保持

	//音源
	//----------------------------------------
	Sound_Manager m_sound_FalledDown = Object_Wood_Fall_Sound;	//倒れた時の音
};
#endif // !WOOD_H
