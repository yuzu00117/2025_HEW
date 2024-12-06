#pragma once

#ifndef ITEM_SPEEDUP_H
#define	ITEM_SPEEDUP_H

#include"include/box2d/box2d.h"

class ItemSpeedUp 
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	




private:
	//アイテムのボディー
	b2Body* m_body;

	//アイテムのサイズ（描画用）
	b2Vec2 m_size;

	//アイテムの透明度
	float m_Alpha;

	//消す予定なのかどうか
	bool	m_destory = false;

};


#endif // !ITEM_SPEEDUP_H
