//-----------------------------------------------------------------------------------------------------
// #name player.h
// #description プレイヤー
// #make 2024/11/19
// #update 2024/11/20
// #comment 追加・修正予定
//          ・コンストラクタでbodyとfixture作ってGetInstanceでつくる
//           
//----------------------------------------------------------------------------------------------------


#ifndef PLAYER_H
#define PLAYER_H



#include"include/box2d/box2d.h"
#include <unordered_map>
#include <string>
#include"tool.h"
#include"display.h"
#include"world_box2d.h"
#include<vector>
#include"Item_SavePoint.h"
#include <memory>  // 追加


enum player_draw_state
{
	null_state,
	player_nomal_state,
	player_jumping_state,
	player_throw_anchor_state,
	player_dameged_state,
	player_walk_state,
	player_normal_attack_state,

	player_dead_state,

};


// **土煙のデータを管理**
struct DustEffect {
	b2Vec2 pos;   // 土煙の発生位置
	int lifeTime; // 残りフレーム数（30フレームで消滅）

	

	DustEffect(b2Vec2 position) : pos(position), lifeTime(30){}
};

class Player
{
public:
	Player();


	~Player();


	// シングルトンのインスタンス取得 　このクラスでは一つのインスタンスしか認めない
	//static Player& GetInstance(b2Vec2 position = b2Vec2(1, 0), b2Vec2 size = b2Vec2(1, 2),b2Vec2 sensor_size=b2Vec2(40,30)) {
	//	static Player instance(position, size,sensor_size); // Initialized only once
	//	return instance;
	//}
	

		// シングルトンインスタンス取得
	static Player& GetInstance() {
		static Player instance;
		return instance;
	}

	 // プレイヤーの初期化
	void Initialize(b2Vec2 position, b2Vec2 body_size, b2Vec2 sensor_size);
	void Update();
	void Draw();
	void Finalize();

	//リスポンやステージ移行の時リセット必要なメンバー変数をリセット
	void	ResetPlayerParameter();

	void Player_sensor_size_change(int anchor_level);



	//フィルターを変換できる
	void updateFixtureFilter(const std::string& category, const std::vector<std::string>& includeMasks);

	//プレイヤーがダメージ受けた瞬間呼び出す
	void Player_Damaged(int Change_to_HP,int invincibletime, const b2Body* attack_body, bool knock_back_only = false);
	//プレイヤーが回復した瞬間呼び出す
	void Palyer_Healed(int heal);

	//無敵時間処理
	void Invincible_time_update();
	//ノックバック処理
	void KnockBack_Update();


	static b2Body* GetOutSidePlayerBody();



	void DrawAnchorLevel3Frame(void);

	//土煙の描画

	 // 土煙の寿命を減らし、30フレーム経過したものを削除**
	void UpdateDustEffect();

	// **プレイヤーが走った際に土煙を記録
	void CreateDustEffect(b2Vec2 playerPos);

	//プレイヤーが歩いた時の土埃
	void DrawDustEffect();

	//アンカーを投げた時に発光するエフェクトを呼び出す関数
	void StartAnchorEffect();

	//たまちゃんの描画
	void DrawTamaChan();

	//アンカーを投げた時にプレイヤーが発光するエフェクト
	void DrawAnchorEffect();

	//アンカーのレベル変動があった際のエフェクト
	void DrawAnchorLevelUpDownEffect();

	int GetAnchorFrameManagement();

	void DrawSoulGetEffect(void);


	//描画用にサイズを持たせておく
	b2Vec2 GetSize() const 
	{
		return m_p_size;
	}
	void SetSize(b2Vec2 size)
	{
		//この処理は絵が上がったらプレイヤーと床の間の隙間を調整する処理
		//size.y = size.y + 1;
		m_p_size = size;
	}

	//描画用にサイズを持たせておく
	b2Vec2 GetSensorSize() const {return m_sensor_size;}
	void SetSensorSize(b2Vec2 sensor_size) {
		m_sensor_size = sensor_size;
	}


	//今ジャンプ中かどうかをセット
	void SetIsJumping(bool flag)
	{
		m_is_jumping = flag;
	}
	//今ジャンプ中かどうかを取得
	bool GetIsJumping(void)
	{
		return m_is_jumping;
	}

	//アンカー投げる時の速度の倍率を取得
	float	GetAnchorThrowing_SpeedUp() { return m_AnchorThrowing_SpeedUp; }
	//アンカー投げる時の速度の倍率をセット
	void	SetAnchorThrowing_SpeedUp(float value) { m_AnchorThrowing_SpeedUp = value; }


	//今の速度を取得
	float	GetSpeed()
	{
		return m_speed;
	}
	//今の速度を加算
	void	SetSpeed(float value)
	{
		m_speed += value;
		if (m_speed < 0)
		{
			m_speed = 0.001f;
		}
	}

	//今のジャンプ力を取得
	b2Vec2	GetJumpForce()
	{
		return m_jump_force;
	}
	//今のジャンプ力を加算
	void	SetJumpForce(b2Vec2 value)
	{
		m_jump_force.x += value.x;
		m_jump_force.y += value.y;
		if (m_jump_force.y > 0)
		{
			m_jump_force.y = -0.001f;
		}
	}


	//プレイヤーが登録した中間地点を取得（nullptrの場合は登録してない）
	ItemSavePoint*	GetRegisteredSavePoint() { return m_registered_SavePoint; }
	//プレイヤーが中間地点を登録（nullptrの場合は登録解除）
	void	RegisterSavePoint(ItemSavePoint* SavePoint) { 
		//現在の登録を解除じゃなくて、新しく登録するなら、今の登録を先に避難させる
		if (SavePoint != nullptr)
		{
			m_prev_registered_SavePoint = m_registered_SavePoint;
		}
		//初回登録の時だけ前の登録した中間地も今回の中間地にする
		if (m_prev_registered_SavePoint == nullptr)
		{
			m_prev_registered_SavePoint = SavePoint;
		}
		m_registered_SavePoint = SavePoint;
	}

	//プレイヤーが前に登録した中間地点を取得（nullptrの場合は登録してない）（ポーズ画面の中間地リスポン用）
	ItemSavePoint* GetPrevRegisteredSavePoint() { 
		return m_prev_registered_SavePoint; }
	//プレイヤーが前に登録した中間地点をセット（nullptrの場合は登録してない）
	void	SetPrevRegisteredSavePoint(ItemSavePoint* SavePoint) {
		m_prev_registered_SavePoint = SavePoint;
	}



	//今のプレイヤーの向きを取得
	// 右向き：1    左向き：0
	bool GetDirection() {
		return m_direction;
	}

	 b2Body*GetPlayerBody(void)
	{
		return m_body;
	}







	b2Vec2 GetSensorSizeLev1_2(void)
	{
		return Sensor_size_Lev1_2;
	}

	b2Vec2 GetSensorSizeLev3(void)
	{
		return Sensor_size_Lev3;
	}


	player_draw_state GetState() { return draw_state; }

	void SetState(player_draw_state state)
	{
		draw_state = state;
	}

	void SetDrawCnt(int cnt)
	{
		draw_cnt = cnt;
	}

	void SetSoulGetEffectType(int effect_type)
	{
		soul_get_effect_type = effect_type;
		soul_get_effect_sheet_cnt = 1;
	}

private:

	//プレイヤーのBodyをもつ
	b2Body* m_body;

	//リスポン用に保存
	b2Vec2 m_body_position;

	//リスポン用に保存
	b2Vec2 m_body_size;

	//リスポン用に保存
	b2Vec2 m_initial_sensor_size;



	//描画用のBodyのサイズもっておく　
	// 理由としてはこいつのサイズをbox2dないで管理するのはだるいから
	//Bodyからfixtureとってきてサイズ確認してー　その時は四角だからー　だるい　してもいいかも
	b2Vec2 m_p_size;

	//センサー用のサイズ
	b2Vec2 m_sensor_size;


	//登録した中間地点
	ItemSavePoint* m_registered_SavePoint = nullptr;
	//前の登録した中間地（ポーズ画面の中間地リスポン用）
	ItemSavePoint* m_prev_registered_SavePoint = nullptr;


	//アンカー投げる時の速度の倍率
	static float m_AnchorThrowing_SpeedUp;
	//ジャンプボタン押されたかどうか（トリガー制御）
	static bool m_jump_pressed;
	//ジャンプ中かどうか
	static bool    m_is_jumping;
	//ジャンプする時の力（ジャンプできる高さに影響）
	static b2Vec2  m_jump_force;

	//横移動スピード
	static float   m_speed;

	//プレイヤーの向き
	// 右向き：true(1)    左向き：false(0)
	static bool		m_direction;

	//アンカーを使用中よフラグ
	bool m_is_use_anchor = false;

	//描画に利用してる　プレイヤーの状態を表したもの
	player_draw_state draw_state;

	//描画用のカウント
	int draw_cnt;

	//無敵時間の管理
	static int invincible_time;

	int player_alpha = 3.0f;


	 b2Vec2 Walk_effect_size = b2Vec2{ 1.0f,1.0f };

	//レベルに応じたセンサーの大きさを記述したもの	displayの変更に伴って　センサーのサイズも自動で変わるようにした
	b2Vec2 Sensor_size_Lev1_2 = b2Vec2(27*calculateScale(DISPLAY_RANGE_TO_SCALE), 21 * calculateScale(DISPLAY_RANGE_TO_SCALE));
	b2Vec2 Sensor_size_Lev3 = b2Vec2(37*calculateScale(DISPLAY_RANGE_TO_SCALE), 29* calculateScale(DISPLAY_RANGE_TO_SCALE));
	
	
	//センサーの管理に使う
	bool sensor_flag;
	int old_anchor_Lev;

	//アンカーのフレームで使っている
	float Anchor_level3_Frame_Sheet_cnt = 0;

	//アンカーを投げた時に光るエフェクト
	int Anchor_Effect_Type = 0;
	//アンカーを投げる時に管理するシートのマックス
	int Max_Anchor_effect_sheet = 0;
	//アンカーを投げる時にプレイヤーが発光する際の現在のシートカウントを管理する関数
	float Anchor_effect_sheet = 0;


	//レベルが変わるたびに発光するエフェクトのタイプの管理
	int Anchor_level_up_down_sheet_type = 0;

	//アンカーのレベルが変わった時に管理する最大シート数
	int Max_Anchor_level_up_Down_effect_sheet = 0;

	//アンカーレベル変わった時のエフェクトを管理する現在のシート数
	float Now_Anchor_level_up_Down_effect_sheet = 0;

	// **土煙エフェクトのリスト**
	std::vector<DustEffect> dustEffects;

	int dustFrameCnt;


	// たまちゃんのアニメーションシートのカウント
	float TamaChanSheetCnt = 0.0f;
	// たまちゃんの向き管理用
	bool is_left = true;
	// アンカーを投げたときのたまちゃん削除フラグ
	bool is_throw_anchor = false;
	// たまちゃんが消えるエフェクトのフラグ
	bool is_tamachan_disappearing = false;
	// たまちゃんが消えるエフェクトのカウント
	float tamachan_disappear_effect_cnt = 0.0f;
	// たまちゃん描画用ポジション
	float tamachan_pos_x = 0.0f;
	float tamachan_pos_y = 0.0f;

	//-----------------------------------------
	//プレイヤーのサウンドの管理
	int walk_state_sound_cnt;


	//------------------------------------------
	//ソウルのゲットした時の関数

	int soul_get_effect_type;

	float soul_get_effect_sheet_cnt;

	//=======================================
	//次Update関数で追加するダメージ表記のリスト
	std::vector<int>add_item_damage_value;


	std::unique_ptr<class ObjectData> m_playerlegdata;  // ユニークポインターに変更
	std::unique_ptr<class ObjectData> m_playerbodydata;  // ユニークポインターに変更
	std::unique_ptr<class ObjectData> m_playersensordata;  // ユニークポインターに変更
	std::unique_ptr<class ObjectData> m_effectsensordata;  // ユニークポインターに変更

};

#endif // !PLAYER_H