//-----------------------------------------------------------------------------------------------------
// #name Item_Manager.h
// #description アイテムの管理(ファクトリー)
// #make 2024/12/28　王泳心
// #update 2024/12/28
// #comment 追加・修正予定
//      
//
// 
//----------------------------------------------------------------------------------------------------

#pragma once

#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include <vector>
#include <memory>
#include"Item_coin.h"
#include"Item_Spirit.h"
#include"Item_Jewel.h"
#include"Item_SavePoint.h"

//アイテムの種類
enum ItemType
{
	ITEM_NONE,		//何もない
	ITEM_SPIRIT,	//ソウル（敵が落とすアイテム）
	ITEM_COIN,//コイン
	ITEM_JEWEL,	//宝石
	ITEM_SAVEPOINT,	//セーブポイント
};



class ItemManager
{

public:
	static ItemManager& GetInstance();


	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	//recovery引数はプレイヤーがアイテムを取るときのソウルの回復値
	void	AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, float Alpha = 1.0f);
	void	AddCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	//Jewel_Type型のtype引数は BLUE, RED, YELLOW のどれかを渡す
	void	AddJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool shape_polygon = true, float Alpha = 1.0f);
	void	AddSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);

	// ID を使ってアイテムを検索
	ItemSpirit* FindItem_Spirit_ByID(int ID);
	ItemCoin* FindItem_Coin_ByID(int ID);
	ItemJewel* FindItem_Jewel_ByID(int ID);
	ItemSavePoint* FindItem_SavePoint_ByID(int ID);

	// 全てのアイテムを初期化
	void InitializeAll();

	// 全てのアイテムを更新
	void UpdateAll();

	// 全てのアイテムを描画
	void DrawAll();

	//全面の描画処理　エフェクトなど
	void DrawFront();

	// 全てのアイテムを破棄
	void FinalizeAll();

	//　全てのソウルアイテムが回収される予定だと設定する
	void	SetCollectSpirit(bool flag);

	//全ての宝石を使う
	void	UseAllJewel();


private:

	std::vector<std::unique_ptr<ItemSpirit>> m_Spirit_List; // ソウルアイテムのリスト
	std::vector<std::unique_ptr<ItemCoin>> m_Coin_List; // コインのリスト
	std::list<std::unique_ptr<ItemJewel>> m_Jewel_List; // 宝石のリスト
	std::vector<std::unique_ptr<ItemSavePoint>> m_SavePoint_List; // セーブポイントのリスト
	//ここにアイテムごとにリストを追加していく感じだねぇー

	ItemManager() = default;
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;

};

#endif // !ITEM_MANAGER_H



