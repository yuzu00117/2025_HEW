//-----------------------------------------------------------------------------------------------------
// #name 1-1_boss.h
// #description bossをつくる　えぐい
// #make 2025/01/07　　永野義也
// #update 2025/01/21
// #comment 追加・修正予定
//          ・特になし
//----------------------------------------------------------------------------------------------------
#ifndef BOSS_1_1_H
#define BOSS_1_1_H

#include"easing.h"
#include"include/box2d/box2d.h"



enum boss_state
{
	wait_state,//待ち状態
	walk_state,//歩き状態
	jump_state,//ジャンプ状態
	charge_attack_state,//チャージ攻撃中
	shock_wave_state,//ショックウェーブをだしている状態
	create_mini_golem_state,//小さなゴーレムを生成している状態

};

class Boss_1_1
{
public:
	Boss_1_1();
	~Boss_1_1();




	void Initialize(b2Vec2 position, b2Vec2 bodysize,bool left);
	void Update();
	void Draw();
	void debugDraw();
	void Finalize();

	void CreateChargeAttack(b2Vec2 attack_size, bool left);

	void DeleteAttackBody();

	b2Vec2 GetBossDrawSize(void)
	{
		return boss_size;
	}
	void SetBossDrawSize(b2Vec2 size)
	{
		boss_size = size;
	}

	b2Body* GetBossBody(void)
	{
		return m_body;
	}

	void SetBossBody(b2Body* body)
	{
		m_body = body;
	}



	b2Vec2 GetAttackDrawSize(void)
	{
		return attack_size;
	}
	void SetAttackDrawSize(b2Vec2 size)
	{
		attack_size = size;
	}

	b2Body* GetAttackBody(void)
	{
		return m_attack_body;
	}

	void SetAttackBody(b2Body* body)
	{
		m_attack_body = body;
	}


private:

	b2Body* m_body;

	b2Body* m_attack_body;

	b2Vec2 boss_size;//描画で使うボスのサイズ

	b2Vec2 attack_size;

	float sheet_cnt;//シートの管理で使っている

	bool  display_shake_flag = true;

	bool left_flag;//プレイヤーがどちらにいるかの描画用

	bool old_left_flag;//振り向きを判別するため



	boss_state now_boss_state;


	static constexpr int Max_Shock_Wave_Sheet = 98;			//衝撃波攻撃の最大フレーム

	static constexpr int Max_Create_Mini_Golem_Sheet = 98;	//ミニゴーレムの生成する最大フレーム

	static constexpr int Max_Charge_Attack_Sheet = 200;		//ため攻撃の生成する最大フレーム

	static constexpr int Max_Walk_Sheet = 72;

	static constexpr float boss_alpha = 3.0f;
};



#endif // !1_1_BOSS_H