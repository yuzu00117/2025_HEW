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
#include<vector>

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

	void Pulling_wood();


	


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
	
	

	//描画用にサイズを持たせておく
	b2Vec2 GetStumpSize() const
	{
		return m_Stump_size;
	}

	void SetStumpSize(b2Vec2 size) {
		m_Stump_size = size;
	}
	//bodyの習得
	b2Body* GetObjectStumpBody()
	{
		return Stump_body;
	}

	void SetObjectStumpBody(b2Body* body)
	{
		Stump_body = body;
	}



	b2Vec2	GetPullingPower() { return m_pulling_power; }
	void	SetPullingPower_With_Multiple(b2Vec2 multiple) {
		m_pulling_power.x *= multiple.x;
		m_pulling_power.y *= multiple.y;	
	}

	//木本体と切り株の間の溶接ジョイント取得
	b2Joint* GetWoodStumpJoint() 
	{
		return m_wood_stump_joint; 
	}

	void	SetWoodStumpJoint(b2Joint* joint)
	{ 
		m_wood_stump_joint = joint; 
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



	void SetLeafSize(b2Vec2 size)
	{
		leaf_size = size;
	}

	b2Vec2 GetLeafSize(void)
	{
		return leaf_size;
	}
	
private:
	int id; // 各インスタンス固有の ID

	b2Vec2 m_pulling_power;

	b2Body* Wood_body;

	b2Vec2 m_Wood_size;


	b2Body* Stump_body;

	b2Vec2 m_Stump_size;

	b2Joint*	m_wood_stump_joint;

	bool	m_destory_joint = false;	//切り株と本体のジョイントを消すかどうか

	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;

	Wood_State	m_state = Wood_Idle;

	int		start_stop_sound_count = 120;	//ゲーム開始直後木が地面まで落ちる時音鳴らさないためのカウントダウン
  
	float	angle_when_pulling_start = 0;	//引っ張り始める時や音鳴らされた直後の木の回転角度を保持

	bool leaf_drop_flag = false;

	static constexpr int  leafDeleteCountdown = 600; // 10秒後に削除 (60FPS x 10秒 = 600フレーム)

	int now_delete_leaf_countDown=0;

	bool m_is_border = true;
	int m_need_level = 0;
	float m_border_alpha = 0;
	const float m_border_alpha_max = 0.75f;
	const float m_border_alpha_min = 0.0f;

	//音源
	//----------------------------------------
	Sound_Manager m_sound_FalledDown = Object_Wood_Fall_Sound;	//倒れた時の音


	b2Vec2 leaf_size;

	std::vector<b2Body*> leaf_bodies; // b2Body* のベクター

	const int NUM_BODIES = 11;

	std::unique_ptr<class ObjectData> m_wood_objectData;  // ユニークポインターに変更
	std::unique_ptr<class ObjectData> m_anchor_point_objectData;  // ユニークポインターに変更
	std::unique_ptr<class ObjectData> m_stump_objectData;  // ユニークポインターに変更


	bool camera_shake_was;//カメラシェイクを既にしたかどうか
};
#endif // !WOOD_H
