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
#include"Item_Healing.h"
#include"Item_Barrier.h"

//アイテムの種類
enum ItemType
{
	ITEM_NONE,		//何もない
	ITEM_SPIRIT,	//ソウル（敵が落とすアイテム）
	ITEM_COIN,//コイン
	ITEM_JEWEL,	//宝石
	ITEM_SAVEPOINT,	//セーブポイント
	ITEM_HEALING, //回復アイテム
	ITEM_BARRIER,	//バリア
};



class ItemManager
{

public:
	static ItemManager& GetInstance();


	//最低必要な引数：position（位置情報）、body_size（サイズ）、angle（回転角度のラジアン）
	// コライダーの形はデフォルトで四角形、円にしたい場合は false を渡す、変更がなければ特に値を渡さなくてもいいよ
	// Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ
	//recovery引数はプレイヤーがアイテムを取るときのソウルの回復値
	void	AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, bool respawning, float Alpha = 1.0f);
	void	AddCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool respawning, bool shape_polygon = true, float Alpha = 1.0f);
	//Jewel_Type型のtype引数は BLUE, RED, YELLOW のどれかを渡す
	void	AddJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool respawning, bool shape_polygon = true, float Alpha = 1.0f);
	void	AddSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool respawning, bool shape_polygon = true, float Alpha = 1.0f);
	void	AddHealing(b2Vec2 position, b2Vec2 body_size, float angle, bool respawning);
	void	AddBarrier(b2Vec2 position, b2Vec2 body_size, float angle, const b2Body* owner_body, float Alpha = 1.0f);

	// ID を使ってアイテムを検索
	ItemSpirit* FindItem_Spirit_ByID(int ID);
	ItemCoin* FindItem_Coin_ByID(int ID);
	ItemJewel* FindItem_Jewel_ByID(int ID);
	ItemSavePoint* FindItem_SavePoint(int ID);
	ItemHealing* FindItem_Healing(int id);
	ItemBarrier* FindItem_Barrier_ByID(int id);
	ItemBarrier* FindItem_Barrier_ByOwnerBody(const b2Body* owner);

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
	//　リスポン用の終了処理
	void Finalize_WhenRespawn();
	//　次のステージに進時の終了処理
	void	Finalize_WhenNextStage();
	//ポーズ画面でリスポン（中間地からやり直す）を選択した時用の終了処理
	void	Finalize_WhenRespawn_SavePoint_GamePause();
	//ポーズ画面でリスタート（最初からやり直す）を選択した時用の終了初期処理
	void	Finalize_WhenRespawn_Initial_GamePause();



	//全ての宝石を使う
	void	UseAllJewel();

	//　リスポン時の初期化処理
	void	Initialize_WhenRespawn();
	//　次のステージに進時の初期化処理
	void	Initialize_WhenNextStage();
	//ポーズ画面でリスポン（中間地からやり直す）を選択した時の初期処理
	void	Initialize_WhenRespawn_SavePoint_GamePause();
	//ポーズ画面でリスタート（最初からやり直す）を選択した時の初期処理
	void	Initialize_WhenRespawn_Initial_GamePause();

	//　中間地を登録した時、宝石に中間地に登録したかどうかを記録させる
	void	SetJewelRegistered_ToSavePoint();
	//　中間地を登録した時、コインに中間地に登録したかどうかを記録させる
	void	SetCoinRegistered_ToSavePoint();


private:

	std::vector<std::unique_ptr<ItemSpirit>> m_Spirit_List; // ソウルアイテムのリスト
	std::vector<std::unique_ptr<ItemCoin>> m_Coin_List; // コインのリスト
	std::list<std::unique_ptr<ItemJewel>> m_Jewel_List; // 宝石のリスト
	std::vector<std::unique_ptr<ItemSavePoint>> m_SavePoint_List; // セーブポイントのリスト
	std::vector<std::unique_ptr<ItemHealing>> m_Healing_List;	//回復アイテムのリスト
	std::list<std::unique_ptr<ItemBarrier>> m_Barrier_List;	//バリアのリスト
	//ここにアイテムごとにリストを追加していく感じだねぇー

	ItemManager() = default;
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;

};

#endif // !ITEM_MANAGER_H



