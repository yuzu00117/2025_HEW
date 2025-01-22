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
	panic_state,//怯み状態
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


	// シングルトンインスタンス取得
	static Boss_1_1& GetInstance() {
		static Boss_1_1 instance;
		return instance;
	}



	void Initialize(b2Vec2 position, b2Vec2 bodysize,bool left);
	void Update();
	void Draw();
	void debugDraw();
	void Finalize();

	void CreateChargeAttack(b2Vec2 attack_size, bool left);//ため攻撃の生成処理

	void CreateShockWave(b2Vec2 attack_size, bool left);//衝撃波攻撃の生成処理
	void ShockWaveUpdate(void);//衝撃波攻撃の更新処理

	void JumpUpdate(void);

	void DeleteAttackBody();//攻撃の判定を削除する処理　現在のところ　衝撃波攻撃とため攻撃の攻撃判定の削除
	


	void CreateMiniGolem(b2Vec2 minigolem_size, bool left);//ミニゴーレムの作成
	void MiniGolemUpdate(void);//ミニゴーレムの更新処理
	void DestroyMiniGolemBody(void);

	b2Body* GetOutSideBody(void);//BodyそとのCPPからとってくるよう

	///-----------------------------------------------------------------------
	//ボス本体
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

	//-------------------------------------------------------------------------------------------
	//攻撃の判定
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
	//-------------------------------------------------------------------------------------------
	//ミニゴーレムの
	b2Body* GetMiniGolemBody(int index)
	{
		return m_mini_golem_body[index];
	}

	void SetMiniGolemBody(b2Body* body,int index)
	{
		m_mini_golem_body[index] = body;
	}


	b2Vec2 GetMiniGolemDrawSize(void)
	{
		return mini_golem_size;
	}
	void SetMiniGolemDrawSize(b2Vec2 size)
	{
		mini_golem_size = size;
	}


	void SetDestroyMiniGolemBody(bool flag, b2Body* body)
	{
		destroy_mini_golem_flag = flag;
		destroy_mini_golem_body = body;
	}

	int GetBossFieldLevel(void)
	{
		return boss_field_level;
	}

	//-------------------------------------------------------------------------------------------
private:

	int boss_field_level=1;//ボスの床の崩壊を管理する関数


	b2Body* m_body;//ボスのボディ
	b2Vec2 boss_size;//描画で使うボスのサイズ

	b2Body* m_attack_body;//攻撃の判定
	b2Vec2 attack_size;//攻撃の判定のサイズ


	b2Body* m_mini_golem_body[2];//ボディ
	b2Vec2 mini_golem_size;
	b2Body* destroy_mini_golem_body;
	bool destroy_mini_golem_flag=false;

	



	float sheet_cnt;//シートの管理で使っている



	bool left_flag;//プレイヤーがどちらにいるかの描画用

	bool old_left_flag;//振り向きを判別するため



	boss_state now_boss_state;

	


	//----------------------------------------------------------------------------------------------
	//衝撃波攻撃
	static constexpr int Max_Shock_Wave_Sheet = 98;			//衝撃波攻撃の最大フレーム
	static constexpr int Shock_Wave_Start_Frame = 50;		//衝撃波の発生フレーム
	static constexpr int Shock_Wave_End_Frame = 71;			//衝撃波の終了フレーム


	static constexpr float Shock_Wave_Speed = 1.5f;			//衝撃波の飛んでいくスピード

	bool Shock_Wave_Fly_flag=false;
	static constexpr int Shock_Wave_time_Frame = 180;
	int Now_Shock_Wave_time_Frame = 0;


	//-------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------
	//ミニゴーレムの生成
	static constexpr int Max_Create_Mini_Golem_Sheet = 98;	//ミニゴーレムの生成する最大フレーム
	static constexpr int Create_Mini_Golem_Start_Frame = 46;

	bool Mini_golem_Create_flag=true;

	//-----------------------------------------------------------------------------------------------
	//ため攻撃のフレーム達
	static constexpr int Max_Charge_Attack_Sheet = 200;		//ため攻撃の生成する最大フレーム
	static constexpr int Charge_Attack_Start_Frame = 67;//ため攻撃のモーションのボディの発生フレーム
	static constexpr int Charge_Attack_End_Frame = 102;//ため攻撃のモーションのボディの発生フレーム

	//-----------------------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------------------------
	//ジャンプモーションの最大フレーム
	static constexpr int Max_Jump_Sheet = 72;
	static constexpr int Jump_Attack_Start_Frame = 52;//ジャンプによって発生する攻撃の発生フレーム
	static constexpr int Jump_Attack_End_Frame = 56;//ジャンプによって発生する攻撃の終了フレーム

	static constexpr int Jump_Start_Frame = 15;//ジャンプの発生フレーム
	static constexpr int Jump_End_Frame = 20;///ジャンプの上昇終了フレーム

	bool Jump_flag = false;
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------
	//怯みのモーションの最大フレーム
	static constexpr int Max_Panic_Sheet = 49;
	//-------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------
	//歩きモーション
	static constexpr int Max_Walk_Sheet = 72;
	//-------------------------------------------------------------------------------------------


	static constexpr float boss_alpha = 3.0f;//ボスのアルファ値
};



#endif // !1_1_BOSS_H