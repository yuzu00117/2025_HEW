#pragma once

#ifndef ITEM_H
#define ITEM_H

#include"world_box2d.h"
#include"renderer.h"

//アイテムの種類
enum ItemType
{
	ITEM_NONE,		//何もない
	ITEM_SPEED_UP,	//スピードアップ
};

class Item
{
public:
	//angleはラジアンですぜ
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	 Item(b2Vec2 position, b2Vec2 body_size, float angle, ItemType type, ID3D11ShaderResourceView* Texture, bool shape_polygon = true, float Alpha = 1.0f);
	 ~Item();
	
	 //ボディーを取得
	 b2Body* GetBody(){ return m_body; }
	 //ボディーをセット
	 void SetBody(b2Body* body){ m_body = body; }


	 // ID を取得する
	 int GetID() const {return m_ID; }
	 // ID をセット
	 void SetID(int ID) { m_ID = ID; }


	 //消える予定なのかを取得
	 bool	GetDestory() { return m_destory; }
	 //インスタンスを削除するかどうかのセット
	 void	SetDestory(bool destory) { m_destory = destory; }

	 //描画用にサイズを持たせておく
	 b2Vec2 GetSize() const{ return m_size; }
	 //描画サイズセット
	 void SetSize(b2Vec2 size){ m_size = size; }


	 //何のアイテムなのかその種類を取得
	 ItemType	GetType() { return m_type; }


	 //何のテクスチャか取得
	 ID3D11ShaderResourceView* GetTexture() { return m_Texture; }
	 //テクスチャをセット
	 void	SetTexture(ID3D11ShaderResourceView* texture) { m_Texture = texture; }


	 //継承先にオーバーライドされる予定
	 virtual	void	Update() = 0;
	 //描画
	 void	Draw();
	
protected:
	//アイテムのボディー
	b2Body*	m_body;

	// 各インスタンス固有の ID
	int m_ID; 

	//アイテムのサイズ（描画用）
	b2Vec2 m_size;
	//アイテムの種類
	ItemType m_type = ITEM_NONE;
	//アイテムの透明度
	float m_Alpha;

	//消す予定なのかどうか
	bool	m_destory = false;

	//テクスチャ
	ID3D11ShaderResourceView* m_Texture = NULL;

};


#endif // !ITEM_H

