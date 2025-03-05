// #name�@break_effect.h
// #description �e�N�X�`���ƃ{�f�B�T�C�Y��n���ƃo���o���ɂ��Ă����@�������ˁ[
// #make 2025/02/06
// #update 2025/02/06
// #comment �ǉ��E�C���\��  �����Ȋ���������
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
#include <random> // �����_��
using namespace DirectX;

// �j�ЃN���X
class Fragment {
public:
    b2Body* body;
    std::chrono::steady_clock::time_point creationTime;
    ID3D11ShaderResourceView* texture;
    int index;

    Fragment(b2Body* b, ID3D11ShaderResourceView* tex, int idx);
    bool ShouldDestroy();
};

// �j�Ѓ}�l�[�W���[ (�V���O���g��)
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
