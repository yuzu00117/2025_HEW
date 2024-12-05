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

// オブジェクトの種類を定義
enum ObjectType {
    NULL_object,

    Object_Wood, // 木
    Object_Rock, // 岩
};

// オブジェクトを管理するクラス
class ObjectManager {
public:
    // シングルトンのインスタンス取得
    static ObjectManager& GetInstance();

    // 木を追加
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const bool&right);

    void AddRock(const b2Vec2& position, const float& radius, const int& need_anchor_level);
      

    // ID を使って木を検索
    wood* FindWoodByID(int id);

    rock* FindRockByID(int id);

    

    // 全てのオブジェクトを初期化
    void InitializeAll();

    // 全てのオブジェクトを更新
    void UpdateAll();

    // 全てのオブジェクトを描画
    void DrawAll();

    // 全てのオブジェクトを破棄
    void FinalizeAll();

private:
    std::vector<std::unique_ptr<wood>> woodList; // 木のリスト

    std::vector < std::unique_ptr<rock>>rockList;

    //ここにオブジェクトごとにリストを追加していく感じだねぇー


    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

#endif // OBJECT_MANAGER_H
