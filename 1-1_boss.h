#ifndef BOSS_1_1_H
#define BOSS_1_1_H

#include"easing.h"
#include"include/box2d/box2d.h"

enum boss_state
{
	wait_state,//待ち状態
	shock_wave_state,//ショックウェーブをだしている状態
	create_mini_golem_state,//小さなゴーレムを生成している状態

};

class Boss_1_1
{
public:
	Boss_1_1();
	~Boss_1_1();




	void Initialize(b2Vec2 position, b2Vec2 bodysize);
	void Update();
	void Draw();
	void debugDraw();
	void Finalize();


	b2Vec2 GetBossSize(void)
	{
		return boss_size;
	}
	void SetBossSize(b2Vec2 size)
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

private:

	b2Body *m_body;

	b2Vec2 boss_size;//描画で使うボスのサイズ

	int sheet_cnt;

};



#endif // !1_1_BOSS_H
