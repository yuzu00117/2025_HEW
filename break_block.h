
//-------------------------------------------------------------------------------------------
// #name　break_block.h
// #description オブジェクトと接触したら壊れるブロック
// #make 2025/02/06
// #update 2025/02/06
// #comment 追加・修正予定  豪快な感じがする
//         
//          
//----------------------------------------------------------------------------------------------------
#ifndef BREAK_BLOCK_H
#define BREAK_BLOCK_H

#include"include/box2d/box2d.h"
#include"sprite.h"
#include"texture.h"
#include <memory>  // 追加


class Break_Block
{
public:
	Break_Block(b2Vec2 Position, b2Vec2 block_size,int divisions_x,int divisions_y,float angle,ID3D11ShaderResourceView* g_Texture);
	~Break_Block();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();


	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//描画用にサイズを持たせておく
	b2Vec2 GetSize() const
	{
		return m_size;
	}

	void SetSize(b2Vec2 size) {
		m_size = size;
	}


	//描画用にサイズを持たせておく
	b2Vec2 GetSensorSize() const
	{
		return m_sensor_size;
	}

	void SetSensorSize(b2Vec2 size) {
		m_sensor_size = size;
	}

	//bodyの習得
	b2Body* GetBody()
	{
		return m_body;
	}

	void SetBody(b2Body* body)
	{
		m_body = body;
	}



	bool GetFlag(void)
	{
		return m_flag;
	}


	void SetFlag(bool flag)
	{
		m_flag = flag;
	}

private:

	b2Body* m_body;

	b2Vec2 m_size;

	b2Vec2 m_sensor_size;

	int id;

	//管理に使う関数
	bool m_flag = false;


	std::unique_ptr<class ObjectData> m_player_objectData;  // ユニークポインターに変更

	std::unique_ptr<class ObjectData> m_objectData;  // ユニークポインターに変更

	ID3D11ShaderResourceView* Texture;

	float angle;

};


#endif //!UI_BLOCK_H
