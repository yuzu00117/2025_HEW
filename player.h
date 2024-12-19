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

#include"world_box2d.h"


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

	void Player_sensor_size_change(int anchor_level);


	static b2Body* GetOutSidePlayerBody();

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



	//今の速度を取得
	float	GetSpeed()
	{
		return m_speed;
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

	void SetPlayerBody(b2Body* player_body)
	{
		m_body = player_body;
	}



	b2Vec2 GetSensorSizeLev1_2(void)
	{
		return Sensor_size_Lev1_2;
	}

	b2Vec2 GetSensorSizeLev3(void)
	{
		return Sensor_size_Lev3;
	}

private:

	//プレイヤーのBodyをもつ
	b2Body* m_body;

	//描画用のBodyのサイズもっておく　
	// 理由としてはこいつのサイズをbox2dないで管理するのはだるいから
	//Bodyからfixtureとってきてサイズ確認してー　その時は四角だからー　だるい　してもいいかも
	b2Vec2 m_p_size;

	//センサー用のサイズ
	b2Vec2 m_sensor_size;


	//ジャンプボタン押されたかどうか（トリガー制御）
	static bool m_jump_pressed;
	//ジャンプ中かどうか
	static bool    m_is_jumping;
	//ジャンプする時の力（ジャンプできる高さに影響）
	b2Vec2  m_jump_force = b2Vec2(0.0f, -0.40f);

	//横移動スピード
	float   m_speed = 0.04f;

	//プレイヤーの向き
	// 右向き：1    左向き：-1
	static bool		m_direction;

	//アンカーを使用中よフラグ
	bool m_is_use_anchor = false;


	//レベルに応じたセンサーの大きさを記述したもの
	b2Vec2 Sensor_size_Lev1_2 = b2Vec2(40, 34);
	b2Vec2 Sensor_size_Lev3 = b2Vec2(80, 68);
};

#endif // !PLAYER_H