//-----------------------------------------------------------------------------------------------------
// #name object_manager
// #description オブジェクトを管理するためのファクトリーのイメージに近い
// #make 2024/11/22　永野義也
// #update 2024/12/03
// #comment 追加・修正予定
//          ・オブジェクトを作るごとに生成する感じ
//----------------------------------------------------------------------------------------------------


#ifndef OBJECT_MANAGER
#define OBJECT_MANAGER


#include"vector"
#include"wood.h"
#include <iostream>

class ObjectManager {
public:
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize) {//木を追加するオブジェクト
        woodList.emplace_back(std::make_unique<wood>(position, woodSize, anchorPointSize));
    }

    void InitializeAll() {
        for (auto& w : woodList) {
            w->Initialize();
        }
    }

    void UpdateAll() {
        for (auto& w : woodList) {
            w->Update();
        }
    }

    void DrawAll() {
        for (auto& w : woodList) {
            w->Draw();
        }
    }

    void FinalizeAll() {
        for (auto& w : woodList) {
            w->Finalize();
        }
        woodList.clear(); // 動的配列をクリアしてメモリ解放
    }

private:
    std::vector<std::unique_ptr<wood>> woodList; // 木のリスト
};

#endif // !OBJECT_MANAGER
