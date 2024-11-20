//-----------------------------------------------------------------------------------------------------
// #name player.h
// #description プレイヤー
// #make 2024/11/19
// #update 2024/11/03
// #comment 追加・修正予定
//          ・コンストラクタでbodyとfixture作ってInitializeでつくる
//           
//----------------------------------------------------------------------------------------------------


#ifndef PLAYER_H
#define PLAYER_H

#include"world_box2d.h"
class Player
{
public:
	Player() {};

	Player(b2Vec2 position, b2Vec2 size);
	~Player();


	// シングルトンのインスタンス取得 　このクラスでは一つのインスタンスしか認めない
	static Player& GetInstance(b2Vec2 position = b2Vec2(0, 0), b2Vec2 size = b2Vec2(1, 1)) {
		static Player instance(position, size); // Initialized only once
		return instance;
	}
	

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	//描画用にサイズを持たせておく
	b2Vec2 GetSize() const { return m_p_size; }
	void SetSize(b2Vec2 size) {
		m_p_size = size;
	}


	//今ジャンプ可能かをコントロールする関数
	void SetJumpFlag(bool flag)
	{
		m_can_jump = flag;
	}

	bool GetCanJump(void)
	{
		return m_can_jump;
	}


	b2Body*GetPlayerBody(void)
	{
		return m_body;
	}

	void SetPlayerBody(b2Body* player_body)
	{
		m_body = player_body;
	}
private:

	//プレイヤーのBodyをもつ
	b2Body* m_body;

	//描画用のBodyのサイズもっておく　
	// 理由としてはこいつのサイズをbox2dないで管理するのはだるいから
	//Bodyからfixtureとってきてサイズ確認してー　その時は四角だからー　だるい　してもいいかも
	b2Vec2 m_p_size;


	//今ジャンプ可能なのか（contactlist.hの方でコントロールしてる）
	bool    m_can_jump = true;

	//ジャンプする時の力（ジャンプできる高さに影響）
	b2Vec2  m_jump_force = b2Vec2(0.0, -0.02);

	//横移動スピード
	float   m_speed = 10.0f;

	//アンカーを使用中よフラグ
	bool m_is_use_anchor = false;
};

#endif // !PLAYER_H