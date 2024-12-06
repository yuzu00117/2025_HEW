#pragma once

#ifndef ITEM_SPEEDUP_H
#define	ITEM_SPEEDUP_H

#include"ItemInterface.h"
#include"texture.h"

class ItemSpeedUp : public Item
{
public:
	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	ItemSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f)
		:Item(position, body_size, angle, ITEM_SPEED_UP, InitTexture(L"asset\\texture\\sample_texture\\speed_up.png"), shape_polygon, Alpha) {}
	

	void	Update()override;



private:

};


#endif // !ITEM_SPEEDUP_H
