#pragma once

#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include <vector>
#include <memory>
#include"Item_SpeedUp.h"

//アイテムの種類
enum ItemType
{
	ITEM_NONE,		//何もない
	ITEM_SPEED_UP,	//スピードアップ
};



class ItemManager
{

public:
	static ItemManager& GetInstance();


	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	void	AddSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	



	// ID を使ってアイテムを検索
	ItemSpeedUp* FindItem_SpeedUp_ByID(int ID);
	
	// 全てのアイテムを初期化
	void InitializeAll();

	// 全てのアイテムを更新
	void UpdateAll();

	// 全てのアイテムを描画
	void DrawAll();

	// 全てのアイテムを破棄
	void FinalizeAll();

	//指定のアイテムを削除
	void DestroyItem(int id, ItemType type);



private:

	std::vector<std::unique_ptr<ItemSpeedUp>> m_SpeedUp_List; // のリスト
	//ここにアイテムごとにリストを追加していく感じだねぇー

	ItemManager() = default;
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;

};

#endif // !ITEM_MANAGER_H



