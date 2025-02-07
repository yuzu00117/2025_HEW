//-----------------------------------------------------------------------------------------------------
// #name Chage_Enemy_filter_and_body.h
// #description 敵のボディとフィルターを新しくする
// #make 2025/02/03　永野義也
// #update 2024/02/03
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------

#ifndef CHANGE_ENEMY_FILTER_AND_BODY_H
#define CHANGE_ENEMY_FILTER_AND_BODY_H
#include"include/box2d/box2d.h"
#include"sprite.h"


class change_enemy_filter_and_body
{
public:
	change_enemy_filter_and_body(b2Vec2 position, b2Vec2 size, b2Vec2 vectol,ID3D11ShaderResourceView* Textur, int texture_x, int texture_y,b2Vec2 vec);//コインのテクスチャ );
	~change_enemy_filter_and_body();


	void Initialize();
	void Update();
	void DrawFront();
	void Finalize();



	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}


	//ボディを取得
	b2Body* GetBody(void)
	{
		return m_body;
	}

	//ボディをセット
	void SetBody(b2Body* body)
	{
		m_body = body;
	}



	//サイズを取得
	b2Vec2 GetSize(void)
	{
		return m_size;
	}
	//サイズをセット
	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}



	void SetDestoryFlag(bool flag)
	{
		Destory_Flag = flag;
	}

private:

	int id;

	b2Body* m_body;

	b2Vec2 m_size;

	ID3D11ShaderResourceView* g_Texture;

	int Splitting_x;
	int Splitting_y;

	int max_sheet_cnt=0;

	float sheet_cnt_now=0;

	float enemy_size_scale = 1.0f;

	float enemy_eising_rate = 1;


	float Destory_Flag=false;


};

#endif // !CHANGE_ENEMY_FILTER_AND_BODY_H
