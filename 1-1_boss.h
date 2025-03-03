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
#include <vector>

#define BOSS_SIZE_SCALE (1.5)
#define ENEMY_GOLEM_SPIRIT_TYPE (Spirit_L)

enum boss_state
{
	wait_state,//待ち状態
	panic_state,//怯み状態
	damage_state,//被弾状態
	die_state,//死亡状態
	down_state,	//ダウン状態
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
	void UpdateCoolTime();
	void UpdateEffectSheetCnt();
	void Draw();
	void debugDraw();//攻撃範囲を表示したりする
	void EffectDraw();
	void BossIconDraw();

	void Finalize();

	void CreateBossCore(b2Vec2 size);//ボスの弱点を露出させた時
	void DestroyBossCore(void);//ボスのアンカーポイントのボディを壊す

	void BossCoreUpdate(void);


	void BossDamaged(void);//ボスの被弾した処理
	void BossDead(void);   //ボスの死亡判定

	void CreateChargeAttack(b2Vec2 attack_size, bool left);//ため攻撃の生成処理

	void CreateShockWave(b2Vec2 attack_size, bool left);//衝撃波攻撃の生成処理
	void ShockWaveUpdate(void);//衝撃波攻撃の更新処理

	void JumpUpdate(void);

	void DeleteAttackBody();//攻撃の判定を削除する処理　現在のところ　衝撃波攻撃とため攻撃の攻撃判定の削除

	void DrawObjectFront();//オブジェクトよりも前に描画する
	


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

	b2Vec2 GetBossRealSize(void)
	{
		return reality_boss_size;
	}

	void SetBossRealSize(b2Vec2 size)
	{
		reality_boss_size = size;
	}

	b2Body* GetBossBody(void)
	{
		return m_body;
	}

	void SetBossBody(b2Body* body)
	{
		m_body = body;
	}

	void SetBossSensorSize(b2Vec2 size)
	{
		boss_sensor_size = size;
	}
	b2Vec2 GetBossSensorSize(void)
	{
		return boss_sensor_size;
	}


	void SetAnchorPointBody(b2Body* body)
	{
		anchorpoint_body = body;
	}

	b2Body* GetAnchorPointBody(void)
	{
		return anchorpoint_body;
	}

	void SetAnchorPointSize(b2Vec2 size)
	{
		anchorpoint_size = size;
	}

	b2Vec2 GetAnchorPointSize(void)
	{
		return anchorpoint_size;
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


	//--------------------------------------------------------------------------------------
	//

	int GetBossFieldLevel(void)//ボスのフィールドの管理
	{
		return boss_field_level;
	}

	//プレイヤーが近くにいるかの管理
	bool GetPlayerisNearbyFlag(void)
	{
		return Player_is_Nearby;
	}
	//プレイヤーが近くにいるかの管理
	void SetPlayerisNearbyFlag(bool flag)
	{
		Player_is_Nearby = flag;
	}
	//
	int GetPlayerNearbylocked(void)
	{
		return Player_is_Nearby_locked;
	}
	void SetPlayerNearbylocked(int cnt)
	{
		Player_is_Nearby_locked = cnt;
	}


	int GetBossHP(void)
	{
		return boss_hp;
	}

	void SetBossHP(int hp)
	{
		boss_hp = hp;
	}

	boss_state GetNowBossState(void)
	{
		return now_boss_state;
	}

	void SetNowBossState(boss_state state,int boss_damege=0)
	{
		now_boss_state = state;

		if (state == damage_state)
		{
			boss_stock=boss_stock-boss_damege;
		}
	}

	//コアが壊れるかどうか
	void SetCoreDeleteFlag(bool flag)
	{
		CoreDeleteFlag = flag;
	}
	

	//コアを引っ張ったかどうか
	void SetCorePullingFlag(bool flag)
	{
		CorePullingFlag = flag;
	}


	// ボス戦の経過時間を設定
  void SetBossElapsedTime(float time) 
  {
    elapsed_time = time;
  }
  
  // ボス戦の経過時間を取得
  float GetBossElapsedTime() const
  {
    return elapsed_time;
  }
  
  void SetBossSheetCnt(int i)
	{
		sheet_cnt = 0;
	}
	

	//-------------------------------------------------------------------------------------------

	void SetShockWaveFrame(float i)
	{
		Now_Shock_Wave_time_Frame = i;
	}
	

	//-----------------------------------------------------------------------------------------

	void SetWaitCoolTime(int i)
	{
		Wait_time = i;
	}


	void SetDisplyInBossFlag(bool flag)
	{
		display_in_boss = flag;
	}
private:

	b2Vec2 Boss_size = b2Vec2(18, 24);

	int boss_hp=3;		   //bossのHP

	int boss_stock=3;

	int dead_cnt;

	int boss_field_level=0;//ボスの床の崩壊を管理する関数


	b2Body* m_body;//ボスのボディ
	b2Vec2 boss_size;//描画で使うボスのサイズ

	b2Vec2 reality_boss_size;//実際のボディのサイズ

	b2Vec2 boss_sensor_size;//ボスのセンサーサイズ

	b2Body* m_attack_body;//攻撃の判定
	b2Vec2 attack_size;//攻撃の判定のサイズ


	b2Body* anchorpoint_body;
	b2Vec2 anchorpoint_size;

	bool CoreDeleteFlag=false;

	bool CorePullingFlag = false;



	//ボスが範囲内にいる
	bool display_in_boss = false;

	b2Body* m_mini_golem_body[2];//ボディ
	bool m_mini_golem_left_flag[2];//ミニゴーレムの向き
	b2Vec2 mini_golem_size;
	b2Body* destroy_mini_golem_body;
	bool destroy_mini_golem_flag=false;

	
	int debug_flag = 0;


	float sheet_cnt;//シートの管理で使っている



	bool left_flag;//プレイヤーがどちらにいるかの描画用

	bool old_left_flag;//振り向きを判別するため



	boss_state now_boss_state;//ボスのステート管理

	bool time_count_flag = false;//時間をカウントするかのフラグ
	float elapsed_time = 0;//経過時間

	int Wait_time = 0;//待ち状態の設定

	//-------------------------------------------------------------------------------------------
	//クールタイムの管理

	//衝撃波攻撃
	static constexpr int Max_Shock_Wave_CoolTime = 600;//衝撃波攻撃最大クールタイム　
	int					 Now_Shock_Wave_CoolTime = 0;  //現在のクールタイム

	//ジャンプ
	static constexpr int Max_Jump_CoolTime = 600;//ジャンプの最大クールタイム　
	int					 Now_Jump_CoolTime = 200;  //現在のクールタイム

	b2Vec2 JumpPower = { 1,-8.0f };

	//小岩生成
	static constexpr int Max_Create_MiniGolem_CoolTime = 1800;//小岩の最大クールタイム　
	int					 Now_Create_MiniGolem_CoolTime = 1500;  //現在のクールタイム

	//チャージ攻撃
	static constexpr int Max_Charge_Attack_CoolTime = 1500;//チャージ攻撃の最大クールタイム　
	int					 Now_Charge_Attack_CoolTime = 1500; //現在のクールタイム

	//歩きモーション
	static constexpr int Max_Max_Walk_CoolTime = 120;//歩きモーションの最大クールタイム　
	int					 Now_Max_Walk_CoolTime = 0; //現在のクールタイム

	//歩きの管理

	b2Vec2 walk_power = { 1.1f,0.3f };

	float targetSpeed = 0;
	float smoothingFactor = 0;
	float newSpeed = 0;
	//--------------------------------------------------------------------------------------------
	//ボスの管理をする　変数
	bool Player_is_Nearby = false;//プレイヤーが近くにいるかの管理
	int Player_is_Nearby_locked;//ボディを消す関係で一時センサーの判定を無効にしたい


	//----------------------------------------------------------------------------------------------
	//衝撃波攻撃
	static constexpr int Max_Shock_Wave_Sheet = 98;			//衝撃波攻撃の最大フレーム
	static constexpr int Shock_Wave_Start_Frame = 50;		//衝撃波の発生フレーム
	static constexpr int Shock_Wave_End_Frame = 71;			//衝撃波の終了フレーム


	static constexpr float Shock_Wave_Speed = 1.5f;			//衝撃波の飛んでいくスピード

	bool Shock_Wave_Fly_flag=false;
	static constexpr int Shock_Wave_time_Frame = 180;
	int Now_Shock_Wave_time_Frame = 0;//ショックウェーブの現在の管理
	bool ShockWaveLeftFlag = true;



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
	static constexpr int Charge_Attack_End_Frame = 102;//ため攻撃のモーションのボディの終了フレーム

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

	//被弾のモーションの最大フレーム
	static constexpr int Max_dameged_Sheet = 25;


	//死亡時のモーションの最大フレーム
	static constexpr int Max_die_Sheet = 72;
	
	//-------------------------------------------------------------------------------------------
	//怯みのモーションの最大フレーム
	static constexpr int Max_Panic_Sheet = 255;

	static constexpr int Max_Panic_Stun_Frame = 255;//やく５秒間
	//-------------------------------------------------------------------------------------------
	//ダウン状態の最大フレーム
	static constexpr int Max_Down_Sheet = 64;

	static constexpr int Max_Down_Frame = 64;//やく５秒間


	//-------------------------------------------------------------------------------------------
	//歩きモーション
	static constexpr int Max_Walk_Sheet = 72;
	//-------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------
	//エフェクト管理用の変数

	float panic_effect_sheet_cnt = 0;
	b2Vec2 panic_effect_size=b2Vec2(4.f, 4.f);

	float charge_attack_effect_sheet_cnt = 0;
	float charge_effect_sheet_cnt = 0;
	float shock_wave_effect_sheet_cnt = 0;

	float mini_golem_break_effect_cnt = 0;

	float dameged_effect_cnt=0;
	//ミニゴーレムはボディが消えるので座標をもっとく
	b2Vec2 mini_golem_delete_effect_position;




	static constexpr int Max_panic_effect_sheet_cnt = Max_Panic_Stun_Frame;
	static constexpr int Max_charge_attack_effect_sheet_cnt = Charge_Attack_End_Frame - Charge_Attack_Start_Frame;
	static constexpr int Max_charge_effect_sheet_cnt = 30;
	static constexpr int Max_shock_wave_effect_sheet_cnt = Shock_Wave_time_Frame;
	static constexpr int Max_mini_golem_break_effect = 16;
	static constexpr int Max_damaged_effect = 90;


	static constexpr float boss_alpha = 3.0f;//ボスのアルファ値
	static constexpr float effect_alpha = 3.0f;//ボスのアルファ値

	//--------------------------------------------------------------------
	//次Update関数で追加するダメージ表記のリスト
	std::vector<int>add_item_damage_value;

};


#endif // !1_1_BOSS_H