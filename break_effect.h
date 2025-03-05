// #name　break_effect.h
// #description テクスチャとボディサイズを渡すとバラバラにしてくれる　豪快だねー
// #make 2025/02/06
// #update 2025/02/06
// #comment 追加・修正予定  豪快な感じがする
//         
//          
//----------------------------------------------------------------------------------------------------

#ifndef PILLAR_FRAGMENTS_MANAGER_H
#define PILLAR_FRAGMENTS_MANAGER_H

#include <vector>
#include <chrono>
#include <d3d11.h>
#include <DirectXMath.h>
#include"include/box2d/box2d.h"
#include"texture.h"
#include"sprite.h"
#include"world_box2d.h"
#include"player_position.h"
#include"create_filter.h"
#include <random> // ランダム
using namespace DirectX;

// 破片クラス
class Fragment {
public:
    b2Body* body;
    std::chrono::steady_clock::time_point creationTime;
    ID3D11ShaderResourceView* texture;
    int index;

    Fragment(b2Body* b, ID3D11ShaderResourceView* tex, int idx);
    bool ShouldDestroy();
};

// 破片マネージャー (シングルトン)
class PillarFragmentsManager {
private:
    std::vector<Fragment> fragments;
    b2World* world;
    int splittingX, splittingY;
    b2Vec2 fragmentSize;

    static PillarFragmentsManager* instance;
    PillarFragmentsManager();

public:
    static PillarFragmentsManager& GetInstance();
    void Init(b2World* w, int sx, int sy);
    void Destroy_Splitting(b2Body* targetBody, ID3D11ShaderResourceView* texture, b2Vec2 size);
    void Destroy_Splitting_only(const b2Vec2 position, ID3D11ShaderResourceView* texture, b2Vec2 size,int x,int y);
    void UpdateFragments();
    void DrawFragments();
    void Finalize();
};

#endif // PILLAR_FRAGMENTS_MANAGER_H
