#pragma once

#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include"ItemInterface.h"

#define	ITEM_MAX (10)

class ItemManager
{
public:
	ItemManager() {}
	~ItemManager() {}

	static ItemManager& GetInstance() {
		static ItemManager instance;
		return instance;
	}


	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）、type（アイテムの種類）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	void	Create(b2Vec2 position, b2Vec2 body_size, float angle, ItemType type, bool shape_polygon = true, float Alpha = 1.0f);
	//bodyのアドレスしか知らない場合
	void	Destory(b2Body* body);
	//アイテムの配列番号を知っている場合
	void	Destory(int item_ID);
	
	void	Update(int ID);
	void	Draw();

private:
	Item* m_p_item[ITEM_MAX];
};
#endif // !ITEM_MANAGER_H



