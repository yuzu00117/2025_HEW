//-----------------------------------------------------------------------------------------------------
// #name anchor.h
// #description アンカーの処理
// #make 2024/11/22 永野義也
// #update 2024/11/22
// #comment 追加・修正予定
//          ・アンカーの処理はひとまずおｋ
//			・チェーンの処理は挙動はいいけど　描画用に変更が必要
//----------------------------------------------------------------------------------------------------
#ifndef ANCHOR_H
#define ANCHOR_H

//アンカーの鎖の部分のボディの入れ物
#define MAX_CHAIN_NUM (40)//チェーンの最大個数

enum AnchorState        //player.cppないのupdateで管理する際に利用している
{
	Nonexistent_state,	//存在しない状態
	Create_wait_draw_cnt_state,//draw_cnt用の調整
	Create_wait_state, //作成するときのモーションまち
	Create_state,		//作成している状態
	Throwing_state,		//投げている状態　 Throwを呼び出す
	Connected_state,	//くっついている状態
	Pulling_state,		//引っ張っている状態
	Deleting_state,


	WaitCreateNormalAttackDraw_cnt_state,//通常攻撃のDrawstate管理用
	WaitCraateNormalAttack_state,//通常攻撃の発生前フレーム消費のため　45フレーム
	CreateNormalAttack_state,//通常攻撃を作成する状態
	NowAttackngNormalAttack,//通常攻撃の攻撃中
	DeletingNormaklAttack_state,//削除した時

};


class Anchor
{
public:
	Anchor();//コンストラクタでは生成しない
	~Anchor();

	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();


	static void SetChainEffect();

	static void DrawChain();

	/**
	* @brief アンカーを投げる処理
	* アンカー自身と目標となる座標を取得して、目標の座標に飛ばす
	*/
	static void ThrowAnchorToAP(float speed_up = 1.0f);

	/**
	 * @brief アンカーを引っ張る
	 */
	static void PullingAnchor(void);

	/**
	 * @brief アンカーを生成する
	 * @param size 生成するアンカーの大きさ
	 */
	static void CreateAnchor(b2Vec2 size);



	/**
	 * @brief アンカーのボディを生成する
	 * @param anchor_size 生成するするアンカー大きさ
	 */
	void CreateAnchorBody(b2Vec2 anchor_size);


	/**
	 * @brief アンカーのボディを削除する
	 */
	void DestroyAnchorBody();//Bodyを削除する

	/**
	 * @brief アンカーを削除する
	 */
	static void DeleteAnchor();

	/**
	 * @brief  ぶつかったら回転ジョイントを付ける
	 */
	static void CreateRotateJoint();

	static void DeleteRotateJoint();


	/*
		
	*/
	static void CreateNormalAttack(b2Vec2 anchor_size, bool right);

	/**
	 * @brief  通常攻撃をするためのボディをつくる
	 * @param anchor_size 　攻撃範囲
	 * @param right 　標準したい向き
	 */
	void CreateNormalAttackAnchorBody(b2Vec2 anchor_size,bool right);


	static void UpdateNormalAttack();

	static void DrawNormalAttack();


	static void DeleteNormalAttackAnchor();

	void DeleteNormalAttackAnchorBody();
	 

	static void DrawAnchorHitEffect(void);





	b2Body* GetAnchorBody(void)
	{
		return m_body;
	}

	void SetAnchorBody(b2Body* anchor_body)
	{
		m_body = anchor_body;
	}


	b2Body* GetNormalAttackAnchorBody(void)
	{
		return m_normal_attack_body;
	}

	void SetNormalAttackAnchorBody(b2Body* anchor_normal_attack_body)
	{
		m_normal_attack_body = anchor_normal_attack_body;
	}

	//描画用にサイズを持たせておく
	b2Vec2 GetSize() const
	{
		return m_anchor_size;
	}

	void SetSize(b2Vec2 size)
	{
		m_anchor_size = size;
	}



	//chainの描画用のサイズ
	b2Vec2 GetChainSize()const
	{
		return m_chain_size;
	}

	void SetChainSize(b2Vec2 size)
	{
		m_chain_size = size;
	}

	static void SetAnchorCreateJointFlag(bool flag);
	static bool GetAnchorCreateJointFlag();
	
	static void SetAnchorState(AnchorState state);
	static AnchorState GetAnchorState();

private:
	//プレイヤーのBodyをもつ
	b2Body* m_body;

	b2Body* m_normal_attack_body;

	b2Vec2 m_anchor_size;

	b2Vec2 m_chain_size;

	bool m_isAnchorCreated = false; // ボディが生成されているか管理

	static bool anchor_create_joint_flag;


	static AnchorState now_anchor_state;

	int anchor_hit_effect_type = 0;//アンカーのタイプの管理レベル別の

	bool anchor_hit_effect_flag=false;//アンカーがのエフェクトがオンになっているか

	int now_anchor_hit_effect_sheet_cnt;//現在のシートすう

	int max_anchor_hit_effect_sheet_cnt;//最大のシートすう

	int anchor_hit_effect_sheet_cnt;

	float anchor_nomal_attack_effect = 0;

	

	int Anchor_Effect_Type;

	float Anchor_effect_sheet[MAX_CHAIN_NUM];

	int Max_Anchor_effect_sheet;

	float chain_effect_scale;
};





#endif // !ANCHOR_H

