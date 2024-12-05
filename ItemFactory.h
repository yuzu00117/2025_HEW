#pragma once

#ifndef ITEM_H
#define ITEM_H

#include"world_box2d.h"
#include"renderer.h"

//アイテムの種類
enum ITEM
{
	ITEM_NONE,		//何もない
	ITEM_SPEED_UP,	//スピードアップ
};

class ItemFactory
{
public:
	 ItemFactory(b2Vec2 position, b2Vec2 scale, float rotate, ITEM type, wchar_t* texture_name, bool shape_polygon = true, float Alpha = 1.0f);
	 ~ItemFactory();
	
	 //描画用にサイズを持たせておく
	 b2Vec2 GetSize() const
	 {
		 return m_size;
	 }
	 void SetSize(b2Vec2 size)
	 {
		 m_size = size;
	 }

	 //何のアイテムなのかその種類を取得
	 ITEM	GetType() { return m_type; }

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

	//アイテムのサイズ（描画用）
	b2Vec2 m_size;
	//アイテムの種類
	ITEM m_type = ITEM_NONE;
	//アイテムの透明度
	float m_Alpha;

	//テクスチャ
	ID3D11ShaderResourceView* m_Texture = NULL;

};


#endif // !ITEM_H

