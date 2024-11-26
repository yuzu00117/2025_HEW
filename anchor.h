#ifndef ANCHOR_H
#define ANCHOR_H


enum AnchorState
{
	Nonexistent_state,	//存在しない状態
	Create_state,		//作成している状態
	Throwing_state,		//投げている状態　 Throwを呼び出す
	Connected_state,	//くっついている状態
	Pulling_state,		//引っ張っている状態
	Deleting_state,
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


	static void DrawChain();

	/**
	* @brief アンカーを投げる処理
	* アンカー自身と目標となる座標を取得して、目標の座標に飛ばす
	*/
	static void ThrowAnchorToAP();

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
	 * @brief 錨部分に追従する鎖部分を作成する関数
	 * @param chain_size 　鎖のサイズ
	 * @param numSegments  鎖の最大数
	 */
	static void CreateChain(b2Vec2 chain_size, int numSegments);

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
	 * @brief チェーンを削除する
	 */
	static void DeleteChain();

	/**
	 * @brief アンカーの生成と削除を管理する関数
	 */
	static void ToggleAnchor();


	/**
	 * @brief  ぶつかったら回転ジョイントを付ける
	 */
	static void CreateRotateJoint();



	b2Body* GetAnchorBody(void)
	{
		return m_body;
	}

	void SetAnchorBody(b2Body* anchor_body)
	{
		m_body = anchor_body;
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

	b2Vec2 m_anchor_size;

	b2Vec2 m_chain_size;

	bool m_isAnchorCreated = false; // ボディが生成されているか管理

	static bool anchor_create_joint_flag;


	static AnchorState now_anchor_state;
};





#endif // !ANCHOR_H

