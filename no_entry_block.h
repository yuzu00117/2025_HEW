//----------------------------------------------------------------------------------------------------
// #name　no_entry_block.h
// #description  立ち入り禁止ブロック
// #make 2025/02/09
// #update 2025/02/09
// #comment 追加・修正予定  
//          
//----------------------------------------------------------------------------------------------------

#ifndef NO_ENTRY_BLOCK_H
#define NO_ENTRY_BLOCK_H

#include"include/box2d/box2d.h"
#include"sprite.h"
#include"texture.h"





class NoEntryBlock
{
public:
	NoEntryBlock(b2Vec2 Position, b2Vec2 block_size,ID3D11ShaderResourceView* g_Texture);
	~NoEntryBlock();


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

	

	int id;

	//管理に使う関数
	bool m_flag = false;

	

	ID3D11ShaderResourceView* Texture;

};


#endif //!EntryBlock
