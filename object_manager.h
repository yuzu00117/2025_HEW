//-----------------------------------------------------------------------------------------------------
// #name object_manager
// #description オブジェクトを管理するためのファクトリーのイメージに近い
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・オブジェクトを作るごとに生成する感じ
//----------------------------------------------------------------------------------------------------


#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <vector>
#include <memory>
#include "wood.h"
#include"rock.h"
#include"one-way_platform.h"
#include"sloping_block.h"

// オブジェクトの種類を定義
enum ObjectType {
    NULL_object,
    Object_Wood, // 木
    Object_Rock, // 岩
    Object_one_way_platform//足場　したからしか乗れない
    
};


// オブジェクトを管理するクラス
class ObjectManager {
public:
    // シングルトンのインスタンス取得
    static ObjectManager& GetInstance();

    // 木を追加
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const bool&right);

    void AddRock(const b2Vec2& position, const float& radius, const int& need_anchor_level);
      
    void AddOne_way_platformList(const b2Vec2& position, const b2Vec2& local_position, const b2Vec2 &size);

    void AddSloping_block(const b2Vec2& position, const b2Vec2& size, const SlopingBlockAspect& aspect);


    // ID を使って木を検索
    wood* FindWoodByID(int id);


    rock* FindRockByID(int id);

    
    one_way_platform* Findone_way_platformByID(int id);

    sloping_block* FindSloping_BlockByID(int id);


    // 全てのオブジェクトを初期化
    void InitializeAll();

    // 全てのオブジェクトを更新
    void UpdateAll();

    // 全てのオブジェクトを描画
    void DrawAll();

    // 全てのオブジェクトを破棄
    void FinalizeAll();

private:
    std::vector<std::unique_ptr<wood>> woodList;
    std::vector<std::unique_ptr<one_way_platform>> one_way_platformList;// 足場のリスト
    std::vector<std::unique_ptr<sloping_block>> sloping_blockList;
    

    std::vector < std::unique_ptr<rock>>rockList;

    //ここにオブジェクトごとにリストを追加していく感じだねぇー


    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

#endif // OBJECT_MANAGER_H
